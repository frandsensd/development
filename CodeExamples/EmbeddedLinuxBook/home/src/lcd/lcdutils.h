
void LCD_erase (int what);
int LCD_init (void);
void LCD_set_cursor (int row, int col);
void LCD_write_string (int row, int col, char *string);
void LCD_write_number (int row, int col, int value, int len);
void LCD_backlight (int on);
void LCD_close (void);

// Erasure constants
#define LCD_LINE 0
#define LCD_SCREEN 1

// Backlight constants
#define BL_OFF 0
#define BL_ON 1
