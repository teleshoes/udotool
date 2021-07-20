#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/uinput.h>

void emitKeystroke(int uinputFD, int type, int code, int val);
void typeString(int uinputFD, const char* str);
void typeChar(int uinputFD, char c);

int DEVICE_INIT_DELAY_MILLIS = 1000;
int KEYSTROKE_DELAY_MILLIS = 10;

int main(int argc, char *argv[]){
  if(argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
    printf("USAGE: %s STRING_TO_TYPE\n", argv[0]);
    exit(1);
  }

  const char* str = argv[1];

  struct uinput_user_dev uinputDev;

  int uinputFD = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  ioctl(uinputFD, UI_SET_EVBIT, EV_KEY);
  for (int i=0; i < 256; i++) {
    ioctl(uinputFD, UI_SET_KEYBIT, i);
  }

  memset(&uinputDev, 0, sizeof(uinputDev));
  snprintf(uinputDev.name, UINPUT_MAX_NAME_SIZE, "legacy uinput keyboard");
  write(uinputFD, &uinputDev, sizeof(uinputDev));

  ioctl(uinputFD, UI_DEV_CREATE);

  usleep(DEVICE_INIT_DELAY_MILLIS * 1000);

  typeString(uinputFD, str);
}

void emitKeystroke(int uinputFD, int type, int code, int val) {
   struct input_event evt;

   evt.type = type;
   evt.code = code;
   evt.value = val;

   evt.time.tv_sec = 0;  //ignored
   evt.time.tv_usec = 0; //ignored

   write(uinputFD, &evt, sizeof(evt));
}

void typeString(int uinputFD, const char* str) {
  emitKeystroke(uinputFD, EV_KEY, KEY_LEFTSHIFT, 1);
  emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);

  emitKeystroke(uinputFD, EV_KEY, KEY_LEFTSHIFT, 0);
  emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);

  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    usleep(KEYSTROKE_DELAY_MILLIS * 1000);
    typeChar(uinputFD, str[i]);
  }
}

void typeChar(int uinputFD, char c) {
  int keyCode = -1;
  int shift = 0;
  switch(c){
    case '\n':               keyCode=KEY_ENTER;         shift=0;  break;
    case '\033':             keyCode=KEY_ESC;           shift=0;  break;
    case '\t':               keyCode=KEY_TAB;           shift=0;  break;
    case ' ':                keyCode=KEY_SPACE;         shift=0;  break;
    case '!':                keyCode=KEY_1;             shift=1;  break;
    case '"':                keyCode=KEY_APOSTROPHE;    shift=1;  break;
    case '#':                keyCode=KEY_3;             shift=1;  break;
    case '$':                keyCode=KEY_4;             shift=1;  break;
    case '%':                keyCode=KEY_5;             shift=1;  break;
    case '&':                keyCode=KEY_7;             shift=1;  break;
    case '\'':               keyCode=KEY_APOSTROPHE;    shift=0;  break;
    case '(':                keyCode=KEY_9;             shift=1;  break;
    case ')':                keyCode=KEY_0;             shift=1;  break;
    case '*':                keyCode=KEY_8;             shift=1;  break;
    case '+':                keyCode=KEY_EQUAL;         shift=1;  break;
    case ',':                keyCode=KEY_COMMA;         shift=0;  break;
    case '-':                keyCode=KEY_MINUS;         shift=0;  break;
    case '.':                keyCode=KEY_DOT;           shift=0;  break;
    case '/':                keyCode=KEY_SLASH;         shift=0;  break;
    case '0':                keyCode=KEY_0;             shift=0;  break;
    case '1':                keyCode=KEY_1;             shift=0;  break;
    case '2':                keyCode=KEY_2;             shift=0;  break;
    case '3':                keyCode=KEY_3;             shift=0;  break;
    case '4':                keyCode=KEY_4;             shift=0;  break;
    case '5':                keyCode=KEY_5;             shift=0;  break;
    case '6':                keyCode=KEY_6;             shift=0;  break;
    case '7':                keyCode=KEY_7;             shift=0;  break;
    case '8':                keyCode=KEY_8;             shift=0;  break;
    case '9':                keyCode=KEY_9;             shift=0;  break;
    case ':':                keyCode=KEY_SEMICOLON;     shift=1;  break;
    case ';':                keyCode=KEY_SEMICOLON;     shift=0;  break;
    case '<':                keyCode=KEY_COMMA;         shift=1;  break;
    case '=':                keyCode=KEY_EQUAL;         shift=0;  break;
    case '>':                keyCode=KEY_DOT;           shift=1;  break;
    case '?':                keyCode=KEY_SLASH;         shift=1;  break;
    case '@':                keyCode=KEY_2;             shift=1;  break;
    case 'A':                keyCode=KEY_A;             shift=1;  break;
    case 'B':                keyCode=KEY_B;             shift=1;  break;
    case 'C':                keyCode=KEY_C;             shift=1;  break;
    case 'D':                keyCode=KEY_D;             shift=1;  break;
    case 'E':                keyCode=KEY_E;             shift=1;  break;
    case 'F':                keyCode=KEY_F;             shift=1;  break;
    case 'G':                keyCode=KEY_G;             shift=1;  break;
    case 'H':                keyCode=KEY_H;             shift=1;  break;
    case 'I':                keyCode=KEY_I;             shift=1;  break;
    case 'J':                keyCode=KEY_J;             shift=1;  break;
    case 'K':                keyCode=KEY_K;             shift=1;  break;
    case 'L':                keyCode=KEY_L;             shift=1;  break;
    case 'M':                keyCode=KEY_M;             shift=1;  break;
    case 'N':                keyCode=KEY_N;             shift=1;  break;
    case 'O':                keyCode=KEY_O;             shift=1;  break;
    case 'P':                keyCode=KEY_P;             shift=1;  break;
    case 'Q':                keyCode=KEY_Q;             shift=1;  break;
    case 'R':                keyCode=KEY_R;             shift=1;  break;
    case 'S':                keyCode=KEY_S;             shift=1;  break;
    case 'T':                keyCode=KEY_T;             shift=1;  break;
    case 'U':                keyCode=KEY_U;             shift=1;  break;
    case 'V':                keyCode=KEY_V;             shift=1;  break;
    case 'W':                keyCode=KEY_W;             shift=1;  break;
    case 'X':                keyCode=KEY_X;             shift=1;  break;
    case 'Y':                keyCode=KEY_Y;             shift=1;  break;
    case 'Z':                keyCode=KEY_Z;             shift=1;  break;
    case '[':                keyCode=KEY_LEFTBRACE;     shift=0;  break;
    case '\\':               keyCode=KEY_BACKSLASH;     shift=0;  break;
    case ']':                keyCode=KEY_RIGHTBRACE;    shift=0;  break;
    case '^':                keyCode=KEY_6;             shift=1;  break;
    case '_':                keyCode=KEY_MINUS;         shift=1;  break;
    case '`':                keyCode=KEY_GRAVE;         shift=0;  break;
    case 'a':                keyCode=KEY_A;             shift=0;  break;
    case 'b':                keyCode=KEY_B;             shift=0;  break;
    case 'c':                keyCode=KEY_C;             shift=0;  break;
    case 'd':                keyCode=KEY_D;             shift=0;  break;
    case 'e':                keyCode=KEY_E;             shift=0;  break;
    case 'f':                keyCode=KEY_F;             shift=0;  break;
    case 'g':                keyCode=KEY_G;             shift=0;  break;
    case 'h':                keyCode=KEY_H;             shift=0;  break;
    case 'i':                keyCode=KEY_I;             shift=0;  break;
    case 'j':                keyCode=KEY_J;             shift=0;  break;
    case 'k':                keyCode=KEY_K;             shift=0;  break;
    case 'l':                keyCode=KEY_L;             shift=0;  break;
    case 'm':                keyCode=KEY_M;             shift=0;  break;
    case 'n':                keyCode=KEY_N;             shift=0;  break;
    case 'o':                keyCode=KEY_O;             shift=0;  break;
    case 'p':                keyCode=KEY_P;             shift=0;  break;
    case 'q':                keyCode=KEY_Q;             shift=0;  break;
    case 'r':                keyCode=KEY_R;             shift=0;  break;
    case 's':                keyCode=KEY_S;             shift=0;  break;
    case 't':                keyCode=KEY_T;             shift=0;  break;
    case 'u':                keyCode=KEY_U;             shift=0;  break;
    case 'v':                keyCode=KEY_V;             shift=0;  break;
    case 'w':                keyCode=KEY_W;             shift=0;  break;
    case 'x':                keyCode=KEY_X;             shift=0;  break;
    case 'y':                keyCode=KEY_Y;             shift=0;  break;
    case 'z':                keyCode=KEY_Z;             shift=0;  break;
    case '{':                keyCode=KEY_LEFTBRACE;     shift=1;  break;
    case '|':                keyCode=KEY_BACKSLASH;     shift=1;  break;
    case '}':                keyCode=KEY_RIGHTBRACE;    shift=1;  break;
    case '~':                keyCode=KEY_GRAVE;         shift=1;  break;
  }
  if(keyCode > 0){
    if(shift){
      emitKeystroke(uinputFD, EV_KEY, KEY_LEFTSHIFT, 1);
      emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);
    }
    emitKeystroke(uinputFD, EV_KEY, keyCode, true);
    emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);

    emitKeystroke(uinputFD, EV_KEY, keyCode, false);
    emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);
    if(shift){
      emitKeystroke(uinputFD, EV_KEY, KEY_LEFTSHIFT, 0);
      emitKeystroke(uinputFD, EV_SYN, SYN_REPORT, 0);
    }
  }
}
