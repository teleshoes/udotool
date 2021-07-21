#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/uinput.h>

constexpr u_int64_t hash(const char* s, size_t index = 0);

void emitKeyEvent(int uinputFD, int keyCode, bool pressed);
void writeEvent(int uinputFD, int type, int code, int val);
void typeString(int uinputFD, const char* str);
void typeChar(int uinputFD, char c);
void pressKeyName(int uinputFD, const char* keyName,
                  bool ctrl = false, bool alt = false,
                  bool super = false, bool forceShift = false);
char* toLower(char* str);

int DEVICE_INIT_DELAY_MILLIS = 200;
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

constexpr u_int64_t hash(const char* s, size_t index) {
  return s + index == nullptr || s[index] == '\0' ? 55 : hash(s, index + 1) * 33 + (unsigned char)(s[index]);
}

void emitKeyEvent(int uinputFD, int keyCode, bool pressed) {
   writeEvent(uinputFD, EV_KEY, keyCode, pressed ? 1 : 0);
   writeEvent(uinputFD, EV_SYN, SYN_REPORT, 0);
}

void writeEvent(int uinputFD, int type, int code, int val) {
   struct input_event evt;

   evt.type = type;
   evt.code = code;
   evt.value = val;

   evt.time.tv_sec = 0;  //ignored
   evt.time.tv_usec = 0; //ignored

   write(uinputFD, &evt, sizeof(evt));
}

void typeString(int uinputFD, const char* str) {
  emitKeyEvent(uinputFD, KEY_LEFTSHIFT, false);

  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    usleep(KEYSTROKE_DELAY_MILLIS * 1000);
    typeChar(uinputFD, str[i]);
  }
}

void typeChar(int uinputFD, char c) {
  const char* keyName;
  switch(c){
    case '\n':    keyName="enter";           break;
    case '\033':  keyName="escape";          break;
    case '\t':    keyName="tab";             break;
    case ' ':     keyName="space";           break;
    case '!':     keyName="bang";            break;
    case '"':     keyName="doublequote";     break;
    case '#':     keyName="hash";            break;
    case '$':     keyName="dollar";          break;
    case '%':     keyName="percent";         break;
    case '&':     keyName="ampersand";       break;
    case '\'':    keyName="apostrophe";      break;
    case '(':     keyName="leftparens";      break;
    case ')':     keyName="rightparens";     break;
    case '*':     keyName="asterisk";        break;
    case '+':     keyName="plus";            break;
    case ',':     keyName="comma";           break;
    case '-':     keyName="dash";            break;
    case '.':     keyName="period";          break;
    case '/':     keyName="slash";           break;
    case '0':     keyName="0";               break;
    case '1':     keyName="1";               break;
    case '2':     keyName="2";               break;
    case '3':     keyName="3";               break;
    case '4':     keyName="4";               break;
    case '5':     keyName="5";               break;
    case '6':     keyName="6";               break;
    case '7':     keyName="7";               break;
    case '8':     keyName="8";               break;
    case '9':     keyName="9";               break;
    case ':':     keyName="colon";           break;
    case ';':     keyName="semicolon";       break;
    case '<':     keyName="less";            break;
    case '=':     keyName="equal";           break;
    case '>':     keyName="greater";         break;
    case '?':     keyName="question";        break;
    case '@':     keyName="at";              break;
    case 'A':     keyName="A";               break;
    case 'B':     keyName="B";               break;
    case 'C':     keyName="C";               break;
    case 'D':     keyName="D";               break;
    case 'E':     keyName="E";               break;
    case 'F':     keyName="F";               break;
    case 'G':     keyName="G";               break;
    case 'H':     keyName="H";               break;
    case 'I':     keyName="I";               break;
    case 'J':     keyName="J";               break;
    case 'K':     keyName="K";               break;
    case 'L':     keyName="L";               break;
    case 'M':     keyName="M";               break;
    case 'N':     keyName="N";               break;
    case 'O':     keyName="O";               break;
    case 'P':     keyName="P";               break;
    case 'Q':     keyName="Q";               break;
    case 'R':     keyName="R";               break;
    case 'S':     keyName="S";               break;
    case 'T':     keyName="T";               break;
    case 'U':     keyName="U";               break;
    case 'V':     keyName="V";               break;
    case 'W':     keyName="W";               break;
    case 'X':     keyName="X";               break;
    case 'Y':     keyName="Y";               break;
    case 'Z':     keyName="Z";               break;
    case '[':     keyName="leftbracket";     break;
    case '\\':    keyName="backslash";       break;
    case ']':     keyName="rightbracket";    break;
    case '^':     keyName="caret";           break;
    case '_':     keyName="underscore";      break;
    case '`':     keyName="grave";           break;
    case 'a':     keyName="a";               break;
    case 'b':     keyName="b";               break;
    case 'c':     keyName="c";               break;
    case 'd':     keyName="d";               break;
    case 'e':     keyName="e";               break;
    case 'f':     keyName="f";               break;
    case 'g':     keyName="g";               break;
    case 'h':     keyName="h";               break;
    case 'i':     keyName="i";               break;
    case 'j':     keyName="j";               break;
    case 'k':     keyName="k";               break;
    case 'l':     keyName="l";               break;
    case 'm':     keyName="m";               break;
    case 'n':     keyName="n";               break;
    case 'o':     keyName="o";               break;
    case 'p':     keyName="p";               break;
    case 'q':     keyName="q";               break;
    case 'r':     keyName="r";               break;
    case 's':     keyName="s";               break;
    case 't':     keyName="t";               break;
    case 'u':     keyName="u";               break;
    case 'v':     keyName="v";               break;
    case 'w':     keyName="w";               break;
    case 'x':     keyName="x";               break;
    case 'y':     keyName="y";               break;
    case 'z':     keyName="z";               break;
    case '{':     keyName="leftbrace";       break;
    case '|':     keyName="pipe";            break;
    case '}':     keyName="rightbrace";      break;
    case '~':     keyName="tilde";           break;
  }

  if(keyName != NULL){
    pressKeyName(uinputFD, keyName, false, false, false, false);
  }
}

void pressKeyName(int uinputFD, const char* keyName, bool ctrl, bool alt, bool super, bool forceShift) {
  int keyCode = -1;
  bool shift = false;

  char* modKeyName = strdup(keyName);
  //force lowercase for all except single letters
  if (strlen(modKeyName) > 1) {
    keyName = toLower(modKeyName);
  }

  switch(hash(modKeyName)){
    case hash("enter"):           keyCode=KEY_ENTER;         shift=false;  break;
    case hash("escape"):          keyCode=KEY_ESC;           shift=false;  break;
    case hash("tab"):             keyCode=KEY_TAB;           shift=false;  break;
    case hash("capslock"):        keyCode=KEY_CAPSLOCK;      shift=false;  break;
    case hash("f1"):              keyCode=KEY_F1;            shift=false;  break;
    case hash("f2"):              keyCode=KEY_F2;            shift=false;  break;
    case hash("f3"):              keyCode=KEY_F3;            shift=false;  break;
    case hash("f4"):              keyCode=KEY_F4;            shift=false;  break;
    case hash("f5"):              keyCode=KEY_F5;            shift=false;  break;
    case hash("f6"):              keyCode=KEY_F6;            shift=false;  break;
    case hash("f7"):              keyCode=KEY_F7;            shift=false;  break;
    case hash("f8"):              keyCode=KEY_F8;            shift=false;  break;
    case hash("f9"):              keyCode=KEY_F9;            shift=false;  break;
    case hash("f10"):             keyCode=KEY_F10;           shift=false;  break;
    case hash("f11"):             keyCode=KEY_F11;           shift=false;  break;
    case hash("f12"):             keyCode=KEY_F12;           shift=false;  break;
    case hash("pageup"):          keyCode=KEY_PAGEUP;        shift=false;  break;
    case hash("pagedown"):        keyCode=KEY_PAGEDOWN;      shift=false;  break;
    case hash("home"):            keyCode=KEY_HOME;          shift=false;  break;
    case hash("end"):             keyCode=KEY_END;           shift=false;  break;
    case hash("insert"):          keyCode=KEY_INSERT;        shift=false;  break;
    case hash("delete"):          keyCode=KEY_DELETE;        shift=false;  break;
    case hash("left"):            keyCode=KEY_LEFT;          shift=false;  break;
    case hash("right"):           keyCode=KEY_RIGHT;         shift=false;  break;
    case hash("up"):              keyCode=KEY_UP;            shift=false;  break;
    case hash("down"):            keyCode=KEY_DOWN;          shift=false;  break;
    case hash("backspace"):       keyCode=KEY_BACKSPACE;     shift=false;  break;
    case hash("space"):           keyCode=KEY_SPACE;         shift=false;  break;
    case hash("bang"):            keyCode=KEY_1;             shift=true;   break;
    case hash("doublequote"):     keyCode=KEY_APOSTROPHE;    shift=true;   break;
    case hash("hash"):            keyCode=KEY_3;             shift=true;   break;
    case hash("dollar"):          keyCode=KEY_4;             shift=true;   break;
    case hash("percent"):         keyCode=KEY_5;             shift=true;   break;
    case hash("ampersand"):       keyCode=KEY_7;             shift=true;   break;
    case hash("apostrophe"):      keyCode=KEY_APOSTROPHE;    shift=false;  break;
    case hash("leftparens"):      keyCode=KEY_9;             shift=true;   break;
    case hash("rightparens"):     keyCode=KEY_0;             shift=true;   break;
    case hash("asterisk"):        keyCode=KEY_8;             shift=true;   break;
    case hash("plus"):            keyCode=KEY_EQUAL;         shift=true;   break;
    case hash("comma"):           keyCode=KEY_COMMA;         shift=false;  break;
    case hash("dash"):            keyCode=KEY_MINUS;         shift=false;  break;
    case hash("period"):          keyCode=KEY_DOT;           shift=false;  break;
    case hash("slash"):           keyCode=KEY_SLASH;         shift=false;  break;
    case hash("0"):               keyCode=KEY_0;             shift=false;  break;
    case hash("1"):               keyCode=KEY_1;             shift=false;  break;
    case hash("2"):               keyCode=KEY_2;             shift=false;  break;
    case hash("3"):               keyCode=KEY_3;             shift=false;  break;
    case hash("4"):               keyCode=KEY_4;             shift=false;  break;
    case hash("5"):               keyCode=KEY_5;             shift=false;  break;
    case hash("6"):               keyCode=KEY_6;             shift=false;  break;
    case hash("7"):               keyCode=KEY_7;             shift=false;  break;
    case hash("8"):               keyCode=KEY_8;             shift=false;  break;
    case hash("9"):               keyCode=KEY_9;             shift=false;  break;
    case hash("colon"):           keyCode=KEY_SEMICOLON;     shift=true;   break;
    case hash("semicolon"):       keyCode=KEY_SEMICOLON;     shift=false;  break;
    case hash("less"):            keyCode=KEY_COMMA;         shift=true;   break;
    case hash("equal"):           keyCode=KEY_EQUAL;         shift=false;  break;
    case hash("greater"):         keyCode=KEY_DOT;           shift=true;   break;
    case hash("question"):        keyCode=KEY_SLASH;         shift=true;   break;
    case hash("at"):              keyCode=KEY_2;             shift=true;   break;
    case hash("A"):               keyCode=KEY_A;             shift=true;  break;
    case hash("B"):               keyCode=KEY_B;             shift=true;  break;
    case hash("C"):               keyCode=KEY_C;             shift=true;  break;
    case hash("D"):               keyCode=KEY_D;             shift=true;  break;
    case hash("E"):               keyCode=KEY_E;             shift=true;  break;
    case hash("F"):               keyCode=KEY_F;             shift=true;  break;
    case hash("G"):               keyCode=KEY_G;             shift=true;  break;
    case hash("H"):               keyCode=KEY_H;             shift=true;  break;
    case hash("I"):               keyCode=KEY_I;             shift=true;  break;
    case hash("J"):               keyCode=KEY_J;             shift=true;  break;
    case hash("K"):               keyCode=KEY_K;             shift=true;  break;
    case hash("L"):               keyCode=KEY_L;             shift=true;  break;
    case hash("M"):               keyCode=KEY_M;             shift=true;  break;
    case hash("N"):               keyCode=KEY_N;             shift=true;  break;
    case hash("O"):               keyCode=KEY_O;             shift=true;  break;
    case hash("P"):               keyCode=KEY_P;             shift=true;  break;
    case hash("Q"):               keyCode=KEY_Q;             shift=true;  break;
    case hash("R"):               keyCode=KEY_R;             shift=true;  break;
    case hash("S"):               keyCode=KEY_S;             shift=true;  break;
    case hash("T"):               keyCode=KEY_T;             shift=true;  break;
    case hash("U"):               keyCode=KEY_U;             shift=true;  break;
    case hash("V"):               keyCode=KEY_V;             shift=true;  break;
    case hash("W"):               keyCode=KEY_W;             shift=true;  break;
    case hash("X"):               keyCode=KEY_X;             shift=true;  break;
    case hash("Y"):               keyCode=KEY_Y;             shift=true;  break;
    case hash("Z"):               keyCode=KEY_Z;             shift=true;  break;
    case hash("leftbracket"):     keyCode=KEY_LEFTBRACE;     shift=false;  break;
    case hash("backslash"):       keyCode=KEY_BACKSLASH;     shift=false;  break;
    case hash("rightbracket"):    keyCode=KEY_RIGHTBRACE;    shift=false;  break;
    case hash("caret"):           keyCode=KEY_6;             shift=true;   break;
    case hash("underscore"):      keyCode=KEY_MINUS;         shift=true;   break;
    case hash("grave"):           keyCode=KEY_GRAVE;         shift=false;  break;
    case hash("a"):               keyCode=KEY_A;             shift=false;  break;
    case hash("b"):               keyCode=KEY_B;             shift=false;  break;
    case hash("c"):               keyCode=KEY_C;             shift=false;  break;
    case hash("d"):               keyCode=KEY_D;             shift=false;  break;
    case hash("e"):               keyCode=KEY_E;             shift=false;  break;
    case hash("f"):               keyCode=KEY_F;             shift=false;  break;
    case hash("g"):               keyCode=KEY_G;             shift=false;  break;
    case hash("h"):               keyCode=KEY_H;             shift=false;  break;
    case hash("i"):               keyCode=KEY_I;             shift=false;  break;
    case hash("j"):               keyCode=KEY_J;             shift=false;  break;
    case hash("k"):               keyCode=KEY_K;             shift=false;  break;
    case hash("l"):               keyCode=KEY_L;             shift=false;  break;
    case hash("m"):               keyCode=KEY_M;             shift=false;  break;
    case hash("n"):               keyCode=KEY_N;             shift=false;  break;
    case hash("o"):               keyCode=KEY_O;             shift=false;  break;
    case hash("p"):               keyCode=KEY_P;             shift=false;  break;
    case hash("q"):               keyCode=KEY_Q;             shift=false;  break;
    case hash("r"):               keyCode=KEY_R;             shift=false;  break;
    case hash("s"):               keyCode=KEY_S;             shift=false;  break;
    case hash("t"):               keyCode=KEY_T;             shift=false;  break;
    case hash("u"):               keyCode=KEY_U;             shift=false;  break;
    case hash("v"):               keyCode=KEY_V;             shift=false;  break;
    case hash("w"):               keyCode=KEY_W;             shift=false;  break;
    case hash("x"):               keyCode=KEY_X;             shift=false;  break;
    case hash("y"):               keyCode=KEY_Y;             shift=false;  break;
    case hash("z"):               keyCode=KEY_Z;             shift=false;  break;
    case hash("leftbrace"):       keyCode=KEY_LEFTBRACE;     shift=true;   break;
    case hash("pipe"):            keyCode=KEY_BACKSLASH;     shift=true;   break;
    case hash("rightbrace"):      keyCode=KEY_RIGHTBRACE;    shift=true;   break;
    case hash("tilde"):           keyCode=KEY_GRAVE;         shift=true;   break;
  }

  if(keyCode > 0){
    if(ctrl){
      emitKeyEvent(uinputFD, KEY_LEFTCTRL, true);
    }
    if(alt){
      emitKeyEvent(uinputFD, KEY_LEFTALT, true);
    }
    if(super){
      emitKeyEvent(uinputFD, KEY_LEFTMETA, true); //they use meta for super instead of meta?
    }
    if(shift || forceShift){
      emitKeyEvent(uinputFD, KEY_LEFTSHIFT, true);
    }

    emitKeyEvent(uinputFD, keyCode, true);

    emitKeyEvent(uinputFD, keyCode, false);

    if(shift || forceShift){
      emitKeyEvent(uinputFD, KEY_LEFTSHIFT, false);
    }
    if(super){
      emitKeyEvent(uinputFD, KEY_LEFTMETA, false);
    }
    if(alt){
      emitKeyEvent(uinputFD, KEY_LEFTALT, false);
    }
    if(ctrl){
      emitKeyEvent(uinputFD, KEY_LEFTCTRL, false);
    }
  }
}

char* toLower(char* str) {
  for(int i = 0; str[i]; i++){
    str[i] = tolower(str[i]);
  }
  return str;
}
