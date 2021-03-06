#pragma once

#include <QtGui>
#include <QtWidgets>

#include <widgets/items.h>
#include <action.h>
#include <observers.h>
#include <utils.h>

#include "palette_export.h"
#include <functional>

typedef std::function<bool(Action& action)> ActionHandler;
Q_DECLARE_METATYPE(ActionHandler);

class CommandPalette : public QMainWindow {
    Q_OBJECT;

public:
    CommandPalette(QWidget* parent = nullptr);

    void show(const QString& name, const QString& placeholder, const QVector<Action>& actions, const QString& closeKey, ActionHandler func);
    void focusOutEvent(QFocusEvent* event) {
        close();
    }

};

class QPaletteFrame : public QFrame {
    Q_OBJECT;

    QString name_;

    QLineEdit* searchbox_;
    Items* items_;
    QShortcut* shortcut_;

    // Registered shortcuts not overriden by Qt::ShortcutOverride event
    QHash<QKeySequence, QShortcut*> registered_keys_;

    bool eventFilter(QObject* obj, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

    void arrowPressed(int delta);

    template<typename T>
    QShortcut* registerShortcut(QKeySequence sequence, T callback) {
        // QKeyEvent treats Meta key as Ctrl, but QAction doesn't.
        QKeySequence keySequence(sequence.toString().replace("Meta+", "Ctrl+"));

        auto* shortcut = new QShortcut(keySequence, this);
        connect(shortcut, &QShortcut::activated, callback);

        registered_keys_.insert(sequence, shortcut);

        return shortcut;
    }

public:
    QPaletteFrame(QWidget* parent, const QString& name, const QVector<Action>& items, const QString& closeKey);

    void setPlaceholderText(const QString& placeholder);

signals:
    bool itemClicked(Action& action);
};

