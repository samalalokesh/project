// password.h

#ifndef _PASSWORD_H_
#define _PASSWORD_H_

/*-------------------------------------------------------
  Password Authentication Function

  Purpose:
  - Prompts user to enter password using keypad
  - Validates entered password
  - Grants access if password is correct
  - Denies access if password is incorrect

  Used in:
  - System startup
  - RTC edit access
  - Emergency mode access
-------------------------------------------------------*/
void password(void);

#endif
