#ifndef TEMPLATE_INFO_TAB_HPP
#define TEMPLATE_INFO_TAB_HPP

#include <QWidget>

namespace Ui {
    class template_info_tab;
    }

class template_info_tab : public QWidget
    {
    Q_OBJECT

public:
    explicit template_info_tab(QWidget *parent = 0);
    ~template_info_tab();

private:
    Ui::template_info_tab *ui;
    };

#endif // TEMPLATE_INFO_TAB_HPP
