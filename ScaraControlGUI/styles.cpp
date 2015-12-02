#include "styles.h"

QString darkWindowTheme = "background-color: rgb(60, 63, 65);"
                          "color: rgb(230, 230, 230);";

QString darkMenuBarTheme = "QMenuBar::item {"
       " background-color: rgb(64, 64, 64);"
    "}"
    "QMenuBar::item:selected { "
        "background-color: rgb(60, 187, 255);"
    "}"
    "QMenu::item:selected {"
       " background-color: rgb(60, 187, 255);"
    "}"
    "border-bottom-color: rgb(255, 255, 255);";

QString darkButtonsTheme = "QPushButton { border: 2px solid #8f8f91; border-radius: 3px; height: 30px; width: 65; }"
                           "QPushButton:hover { background-color: rgb(60, 187, 255); }";

QString darkLineEditTheme = "QLineEdit {"
                            "border: 2px solid #8f8f91;"
                            "border-radius: 1px;"
                        "}"
                        "QLineEdit:hover {"
                            "background-color: rgb(64, 64, 64);"
                            "border: 2px solid #8f8f91;"
                            "border-color: rgb(60, 187, 255);"
                        "}"

                        "QLineEdit:focus {"
                         "   background-color: rgb(64, 64, 64);"
                           " border: 2px solid #8f8f91;"
                            "border-color: rgb(60, 187, 255);"
                        "}";

QString darkComboBoxTheme = "QComboBox {"
        "border: 2px solid #8f8f91;"
        "border-radius: 1px;"
    "}"
    "QComboBox:hover {"
        "background-color: rgb(64, 64, 64);"
        "border: 2px solid #8f8f91;"
        "border-color: rgb(60, 187, 255);"
    "}"
    "QComboBox:focus {"
        "background-color: rgb(64, 64, 64);"
        "border: 2px solid #8f8f91;"
        "border-color: rgb(60, 187, 255);"
    "}";

QString darkErrorBoxTheme = "QPushButton {"
                        "color: rgb(230, 230, 230);"
                        "border: 2px solid #8f8f91;"
                        "border-radius: 3px;"
                        "height: 30px;"
                        "width: 60px;"
                     "}"
                     "QPushButton:hover {"
                        "background-color: rgb(60, 187, 255);"
                     "}"
                     "QMessageBox {"
                        "background-color: rgb(64, 64, 64);"
                     "}"
                     "QLabel {"
                        "color: rgb(230, 230, 230);"
                     "}";

QString currentWindowTheme = darkWindowTheme;
QString currentMenuBarTheme = darkMenuBarTheme;
QString currentButtonTheme = darkButtonsTheme;
QString currentLineEditTheme = darkLineEditTheme;
QString currentComboBoxTheme = darkComboBoxTheme;
QString currentErrorBoxTheme = darkErrorBoxTheme;















