#include "styles.h"

QString darkHighlightLineTheme = "#323232";

QString darkLineNumberingRectTheme = "#313335";

QString darkLineNumberingPenTheme = "#808080";

QString darkHeaderTheme = "QHeaderView::section {background-color: rgb(61, 56, 54);"
                          "height: 35px;"
                          "margin: 0;"
                          "padding-left: 10px;"
                          "border: 1px solid rgb(41, 43, 45);"
                          "border-right: 0px solid rgb(41, 43, 45);}";

QString darkWindowTheme = "background-color: rgb(60, 63, 65);"
                          "color: rgb(230, 230, 230);";

QString darkCodeEditorTheme = "border: 1px solid rgb(41, 43, 45);"
                              "background-color: rgb(43, 43, 43);"
                              "border-top: 0px solid rgb(41, 43, 45);";

QString darkMenuBarTheme = "QMenuBar::item {"
                           " background-color: rgb(64, 64, 64);"
                           "}"
                           "QMenuBar::item:selected { "
                           "background-color: rgb(60, 187, 255);"
                           "}"
                           "QMenu::item:selected {"
                           " background-color: rgb(60, 187, 255);"
                           "}"
                           "QMenu::item:disabled:selected {"
                           "background-color: #808080;"
                           "}"
                           "border-bottom-color: rgb(255, 255, 255);";

QString darkButtonsTheme = "QPushButton { border: 2px solid #8f8f91; border-radius: 3px; height: 30px; background-color: rgb(60, 63, 65); }"
                           "QPushButton:hover { background-color: rgb(60, 187, 255); }";

QString connectButtonsTheme = "QPushButton { border: 2px solid #8f8f91; border-radius: 3px; height: 30px; background-color: green; }";
QString disconnectButtonsTheme = "QPushButton { border: 2px solid #8f8f91; border-radius: 3px; height: 30px; background-color: darkred; }";
QString moveButtonsTheme = "QPushButton { border: 2px solid #8f8f91; border-radius: 3px; height: 30px; background-color: rgb(60, 63, 65); }"
        "QPushButton:pressed { background-color: green;}";

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

QString darkTabWidgetTheme = "QTabWidget::pane {"
                             "border-top: 4px solid rgb(61, 56, 54);"
                             "border-left: 1px solid rgb(41, 43, 45);"
                             "background-color: rgb(255, 244, 89);"
                             "}"

                             "QTabBar::tab { "
                             "border: 1px solid rgb(41, 43, 45);"
                             "background-color: rgb(60, 63, 65);"
                             "height: 30px;"
                             "padding-right: 5px;"
                             " padding-left: 5px;"
                             "}"

                             "QTabBar::tab:selected {"
                             "background-color: rgb(61, 56, 54);"
                             " border-bottom-color: rgb(61, 56, 54);"
                             "}"
                             "QTabBar::tab:movable {"
                             "background-color: rgb(61, 56, 54);"
                             "color: white;"
                             "}";

QString darkTreeWidgetTheme = "border: 0;";

QString darkToolTipTheme = "QToolTip {"
                           "background-color: rgb(60, 63, 65);"
                           "color: rgb(230, 230, 230);"
                           "border: 1px solid rgb(41, 43, 45);"
                           "}";

QString darkLogoTheme = "border: 1px solid rgb(41, 43, 45);"
                        "background-color: rgb(43, 43, 43);"
                        "border-top: 0px solid rgb(41, 43, 45);";


QString darkGroupBoxtTheme = "QGroupBox {"
                             "border: 2px solid #8f8f91;"
                             " border-radius: 5px;"
                             "}"
                             "QGroupBox::title {"
                             "subcontrol-origin: margin;"
                             "subcontrol-position: top center;"
                             " padding: 0 3px;"
                             "}";

QString darkProjectEditorTheme = "border: 1px solid rgb(41, 43, 45);"
        "background-color: rgb(43, 43, 43);"
        "border-top: 0px solid rgb(41, 43, 45);";

QString currentProjectEditorTheme = darkProjectEditorTheme;
QString currentGroupBoxTheme = darkGroupBoxtTheme;
QString currentLogoTheme = darkLogoTheme;
QString currentToolTipTheme = darkToolTipTheme;
QString currentHeaderTheme = darkHeaderTheme;
QString currentTreeWigetTheme = darkTreeWidgetTheme;
QString currentCodeEditorTheme = darkCodeEditorTheme;
QString currentTabWidgetTheme = darkTabWidgetTheme;
QString currentWindowTheme = darkWindowTheme;
QString currentMenuBarTheme = darkMenuBarTheme;
QString currentButtonTheme = darkButtonsTheme;
QString currentLineEditTheme = darkLineEditTheme;
QString currentComboBoxTheme = darkComboBoxTheme;
QString currentErrorBoxTheme = darkErrorBoxTheme;















