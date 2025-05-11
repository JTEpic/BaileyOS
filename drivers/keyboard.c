#include "keyboard.h"
#include "serials.h"
#include "pic.h"
#include "strings.h"

// Global variable to store last scancode
static volatile unsigned char last_scancode;
//bool last_E0=false;

void handle_keyboard_interrupt() {
    last_scancode = inb(KEYBOARD_DATA);
    outb(PIC1_COMMAND, 0x20); // Send master PIC, controlling keyboard, EOI (end of interrupt)

    //if(last_scancode==0xE0&&){
    //    serial_message("E0 clicked\n");
    //    last_E0=true;
    //}else
    //    last_E0=false;
}

void clear_keyboard_interrupt(){
    inb(KEYBOARD_DATA);
    outb(PIC1_COMMAND, 0x20); // Send master PIC, controlling keyboard, EOI (end of interrupt)
    //overflows through handle_keyboard_interrupt() then reset last_scancode
    last_scancode=0;
}

unsigned char get_last_scancode() {
    unsigned char scancode = last_scancode;
    last_scancode = 0; // Clear after reading
    return scancode;
}

char* scancode_to_char(unsigned char scancode) {
    //if (scancode == 0x1E) return 'A';
    //if (scancode == 0x30) return 'B';
    //return '~'; // Unknown

    switch (scancode) {
        case 0x0:
            return "ERROR";
        case 0x1:
            return "Esc";
        case 0x2:
            return "1";
        case 0x3:
            return "2";
        case 0x4:
            return "3";
        case 0x5:
            return "4";
        case 0x6:
            return "5";
        case 0x7:
            return "6";
        case 0x8:
            return "7";
        case 0x9:
            return "8";
        case 0x0A:
            return "9";
        case 0x0B:
            return "0";
        case 0x0C:
            return "-";
        case 0x0D:
            return "+";
        case 0x0E:
            return "Backspace";
        case 0x0F:
            return "Tab";
        case 0x10:
            return "Q";
        case 0x11:
            return "W";
        case 0x12:
            return "E";
        case 0x13:
            return "R";
        case 0x14:
            return "T";
        case 0x15:
            return "Y";
        case 0x16:
            return "U";
        case 0x17:
            return "I";
        case 0x18:
            return "O";
        case 0x19:
            return "P";
		case 0x1A:
			return "[";
		case 0x1B:
		    return "]";
		case 0x1C:
			return "Enter"; //KeyEnter 0xE0
		case 0x1D:
			return "LCtrl"; //RCtrl 0xE0
		case 0x1E:
			return "A";
		case 0x1F:
			return "S";
        case 0x20:
            return "D";
        case 0x21:
            return "F";
        case 0x22:
            return "G";
        case 0x23:
            return "H";
        case 0x24:
            return "J";
        case 0x25:
            return "K";
        case 0x26:
            return "L";
        case 0x27:
            return ";";
        case 0x28:
            return "'";
        case 0x29:
            return "`";
		case 0x2A:
			return "LShift";
		case 0x2B:
			return "\\";
		case 0x2C:
			return "Z";
		case 0x2D:
			return "X";
		case 0x2E:
			return "C";
		case 0x2F:
			return "V";
        case 0x30:
            return "B";
        case 0x31:
            return "N";
        case 0x32:
            return "M";
        case 0x33:
            return ",";
        case 0x34:
            return ".";
        case 0x35:
            return "/"; //Key/ 0xE0
        case 0x36:
            return "RShift";
        case 0x37:
            return "Keypad *";
        case 0x38:
            return "LAlt"; //RAlt 0xE0
        case 0x39:
            return "Spc";
        case 0x3A:
            return "CapsLock";
        case 0x3B:
            return "F1";
        case 0x3C:
            return "F2";
        case 0x3D:
            return "F3";
        case 0x3E:
            return "F4";
        case 0x3F:
            return "F5";
        case 0x40:
            return "F6";
        case 0x41:
            return "F7";
        case 0x42:
            return "F8";
        case 0x43:
            return "F9";
        case 0x44:
            return "F10";
        case 0x45:
            return "NumberLock";
        case 0x46:
            return "ScrollLock";

        case 0x47:
            return "Key7"; //Home 0xE0
        case 0x48:
            return "Key8"; //CUp 0xE0
        case 0x49:
            return "Key9"; //PUp 0xE0
        case 0x4A:
            return "Key-";
        case 0x4B:
            return "Key4"; //CLeft 0xE0
        case 0x4C:
            return "Key5";
        case 0x4D:
            return "Key6"; //CRight 0xE0
        case 0x4E:
            return "Key+";
        case 0x4F:
            return "Key1"; //End 0xE0
        case 0x50:
            return "Key2"; //CDown 0xE0
        case 0x51:
            return "Key3"; //PDown 0xE0
        case 0x52:
            return "Key0"; //Insert 0xE0
        case 0x53:
            return "Key."; //Delete 0xE0

        case 0x57:
            return "F11";
        case 0x58:
            return "F12";

        case 0x5B:
            return "LeftStart"; //0xE0
        case 0x5C:
            return "RightStart"; //0xE0
        default:
            /* 'keyup' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                return "Unknown key down";
            } else if (scancode <= 0x53 + 0x80) {
                //(global var) static array to not deallocate on return, initalization runs once, inc size if needed
                static char keyUp[20];
                keyUp[0]='\0';

                //combine char arrays,key up and key
                strcat(keyUp,"key up ");
                strcat(keyUp,scancode_to_char(scancode - 0x80));

                return keyUp;
            } else if(scancode==(0x57+0x80))
                return "key up F11";
            else if(scancode==(0x58+0x80))
                return "key up F12";
            else if(scancode==(0x5B+0x80))
                return "key up LeftStart";
            else if(scancode==(0x5C+0x80))
                return "key up RightStart";
            else return "Unknown key up";
    }
}