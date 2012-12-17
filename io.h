#ifndef IO_H
#define IO_H

enum error {
	E_OK,
	E_NO_FILE,
	E_BAD_FILE,
	E_BAD_STATE_CHANGE,
	E_BAD_CONFIG,
	E_INTERNAL_ERROR,
	E_NO_MEM,
	E_UNKNOWN,
	NUM_ERRORS,
};

void		debug     (int level, const char *format,...);
void		error     (enum error code, const char *format,...);
int		input_waiting(void);

#endif	/* !IO_H */
