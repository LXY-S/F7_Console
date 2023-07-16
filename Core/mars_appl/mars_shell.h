//
// Created by Mars on 2023/7/8.
//

#ifndef MARS_SHELL_H
#define MARS_SHELL_H

void mars_shell_init(void (*init)(void), void (*output)(unsigned char *data, unsigned short len));
void mars_shell_input(unsigned char data);
void mars_shell_run(void);

#endif //MARS_SHELL_H
