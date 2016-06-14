#ifndef EXAMPLEINFOTAB_HPP
#define EXAMPLEINFOTAB_HPP

#include <QWidget>

namespace Ui {
    class exampleInfoTab;
    }

class exampleInfoTab : public QWidget
    {
    Q_OBJECT

public:
    explicit exampleInfoTab(QWidget *parent = 0);
    ~exampleInfoTab();

private:
    Ui::exampleInfoTab *ui;
    };

#endif // EXAMPLEINFOTAB_HPP
