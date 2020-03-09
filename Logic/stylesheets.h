#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>

static QString styleSheets = QString(
            "QPushButton:hover:!pressed {"
            "background-color: #e9967a;"
            "}"
            "QPushButton {"
            "background-color: #3874d9;"
            "color: white;"
            "border-style: outset;"
            "border-width: 2px;"
            "border-radius: 10px;"
            "padding: 6px; "
            "}"
            );

static QString ComboStyle = QString(
            "QComboBox:hover:!pressed {"
            "background-color: #4682b4;"
            "}"
            "QComboBox {"
            "background-color: #000080;"
            "color: white;"
            "border-width: 2px;"
            "border-style: outset;"
            "border-radius: 10px;"
            "border-color: white;"
            "padding: 6px"
            "}"
            "");

static QString messageBoxStyle = QString ("QMessageBox {background-color: #a9a9a9;"
                                          "}"
                                          "QPushButton {background-color: #000080;"
                                          "color: white;"
                                          "border-style: outset;"
                                          "border-width: 2px;"
                                          "border-radius: 10px;"
                                          "padding: 6px }"
                                          "QPushButton:hover:!pressed {"
                                          "background-color: #4682b4"
                                          "}");

#endif // STYLESHEETS_H
