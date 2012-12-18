#include <pthread.h>
#include <stdint.h>
#include <assert.h>

#include "io.h"
#include "player.h"
#include "audio.h"

struct player_context {
	enum player_state state;
	int		ao_driver_id;
	ao_option      *ao_options;
	struct audio   *au;

        pthread_mutex_t state_mutex;
        pthread_cond_t state_changed;
};


static void player_set_state(struct player_context *play, enum player_state new);


int
player_init(struct player_context **play,
	    int ao_driver_id,
	    ao_option * ao_options)
{
	int		failure = 0;

	if (*play == NULL) {
		*play = calloc(1, sizeof(struct player_context));
		if (*play == NULL) {
			failure = -1;
		}
	}
	if (!failure) 
            failure = pthread_mutex_init(&(*play)->state_mutex, NULL); 
        if (!failure)
            failure = pthread_cond_init(&(*play)->state_changed, NULL);
        if (!failure) {
		(*play)->ao_driver_id = ao_driver_id;
		(*play)->ao_options = ao_options;
	}
	return failure;
}

void
player_free(struct player_context *play)
{
    pthread_mutex_destroy(&(play->state_mutex));
    pthread_cond_destroy(&(play->state_changed));
}

enum error
player_eject(struct player_context *play)
{
	enum error	result;

	switch (play->state) {
	case STOPPED:
	case PLAYING:
	case VOID:
		if (play->au != NULL) {
			audio_unload(play->au);
			play->au = NULL;
		}
		player_set_state(play, EJECTED);
		result = E_OK;
		debug(0, "player ejected");
		break;
	case EJECTED:
		/* Ejecting while ejected is harmless and common */
		break;
	case SHUTTING_DOWN:
		result = error(E_BAD_STATE, "player is shutting down");
		break;
	default:
		result = error(E_BAD_STATE, "unknown state");
	}

	return result;
}

enum error
player_play(struct player_context *play)
{
	enum error	result;

	switch (play->state) {
	case STOPPED:
		player_set_state(play, PLAYING);
		result = E_OK;
		break;
	case PLAYING:
		result = error(E_BAD_STATE, "already playing");
		break;
	case EJECTED:
		result = error(E_BAD_STATE, "nothing loaded");
		break;
	case VOID:
		result = error(E_BAD_STATE, "init only to ejected");
		break;
	case SHUTTING_DOWN:
		result = error(E_BAD_STATE, "player is shutting down");
		break;
	default:
		result = error(E_BAD_STATE, "unknown state");
		break;
	}

	return result;
}

enum error
player_stop(struct player_context *play)
{
	enum error	result;

	switch (play->state) {
	case PLAYING:
		player_set_state(play, STOPPED);
		result = E_OK;
		break;
	case STOPPED:
		result = error(E_BAD_STATE, "already stopped");
		break;
	case EJECTED:
		result = error(E_BAD_STATE, "can't stop - nothing loaded");
		break;
	case SHUTTING_DOWN:
		result = error(E_BAD_STATE, "player is shutting down");
		break;
	default:
		result = error(E_BAD_STATE, "unknown state");
	}

	return result;
}

enum error
player_load(struct player_context *play, const char *filename)
{
	enum error	result;
	enum audio_init_err err;

	player_eject(play);

	err = audio_load(&(play->au),
			 filename,
			 play->ao_driver_id,
			 play->ao_options);
	if (err) {
		switch (err) {
		case E_AINIT_OPEN_INPUT:
			result = error(E_NO_FILE, filename);
			break;
		case E_AINIT_FIND_STREAM_INFO:
			result = error(E_BAD_FILE, "can't find stream info");
			break;
		case E_AINIT_DEVICE_OPEN_FAIL:
			result = error(E_BAD_FILE, "can't open device");
			break;
		case E_AINIT_NO_STREAM:
			result = error(E_BAD_FILE, "can't find stream");
			break;
		case E_AINIT_CANNOT_ALLOC_AUDIO:
			result = error(E_NO_MEM, "can't alloc audio structure");
			break;
		case E_AINIT_CANNOT_ALLOC_PACKET:
			result = error(E_NO_MEM, "can't alloc packet");
			break;
		case E_AINIT_CANNOT_ALLOC_FRAME:
			result = error(E_NO_MEM, "can't alloc frame");
			break;
		default:
			result = error(E_UNKNOWN, "unknown error");
			break;
		}
		player_eject(play);
	} else {
		player_set_state(play, STOPPED);
		result = E_OK;
	}

	return result;
}

void
player_on_state_change(struct player_context *play, void (*cb)(struct player_context *))
{
        pthread_mutex_lock(&play->state_mutex);
        pthread_cond_wait(&play->state_changed, &play->state_mutex);

        cb(play);
        
        pthread_mutex_unlock(&play->state_mutex);
}

void
player_update(struct player_context *play)
{
	if (play->state == PLAYING) {
		assert(play->au != NULL);

		int		err;

		err = audio_play_frame(play->au);
		if (err) {
			switch (err) {
			case E_PLAY_EOF:
				break;
			case E_PLAY_DECODE_ERR:
				error(E_BAD_FILE, "decode error");
				break;
			default:
				error(E_UNKNOWN, "unknown error");
				break;
			}
			player_eject(play);

		}
	}
}

enum error
player_shutdown(struct player_context *play)
{
	enum error result;

        result = player_eject(play);
	player_set_state(play, SHUTTING_DOWN);

        return result;
}

enum player_state
player_state(struct player_context *play)
{
	return play->state;
}

static void player_set_state(struct player_context *play, enum player_state new)
{
    pthread_mutex_lock(&play->state_mutex);

    play->state = new;
    pthread_cond_signal(&play->state_changed);

    pthread_mutex_unlock(&play->state_mutex);
}
