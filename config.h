#ifndef CONFIG_H_
#define CONFIG_H_

#define VERSION             "fanfriend/0.0.1" // let 'em know who we are
#define USB_MFG_NAME        L"garydotcool"
#define USB_PRD_NAME        L"FanFriend USB Fan Controller"
#define USB_VENDOR          0x03EB
#define USB_PRODUCT         0x204F
#define MAX_FANS            6                 // Number of fans to allocate
#define INIT_FAN_SPEED      255               // How fast the fans should spin on MCL startup (0 <= n <= 255)
#define LED_PIN             7

#endif // CONFIG_H_