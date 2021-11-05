HANDLE hconsole;

// ##### LOG COLORS #############################
#define RED  									                \
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
	SetConsoleTextAttribute(hconsole, 12);

#define GREEN  									              \
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
	SetConsoleTextAttribute(hconsole, 2);

#define RESET									                \
	hconsole = GetStdHandle(STD_OUTPUT_HANDLE);	\
	SetConsoleTextAttribute(hconsole, 7);

#define LOG_MESS(typeInfo, ...)					    \
	if (strcmp(#typeInfo, "ERROR") == 0) {		\
		RED 									                  \
	}  											                  \
	if (strcmp(#typeInfo, "GOOD") == 0) {		  \
		GREEN 									                \
	}  											                  \
	printf(__VA_ARGS__);						          \
	RESET

