#ifndef TEMPLATE_EDIT_TAB_HPP
#define TEMPLATE_EDIT_TAB_HPP

#include <QWidget>

namespace Ui {
    class template_edit_tab;
    }

class template_edit_tab : public QWidget
    {
    Q_OBJECT

public:
    explicit template_edit_tab(QWidget *parent = 0);
    ~template_edit_tab();

private:
    Ui::template_edit_tab *ui;
    };

#endif // TEMPLATE_EDIT_TAB_HPP
