#ifndef TM1700_H
#define TM1700_H


void lcd_io_init();
void show_temp(char temp);
void un_diplay_temp();
void disp_init();
void open_display();
void close_display();
void display_icon_wendu();
void un_display_icon_wendu();
void refresh_screen();
void display_icon_huomiao();
void un_display_icon_huomiao();
void set_icon(unsigned char icon ,unsigned char on_off);

#endif