#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    int IsValidSymbol(QChar symbol, bool IsRus);
    int findMin(QString str, int arr[], int len);
    bool IsUsed(int arr[], int ind, int len);
    QString Column_Improved(int tt_length, int tk_length, QString true_text, QString true_key, bool IsCypher);
    QString Vigener_SelfGen(int tt_length, int tk_length, QString true_text, QString true_key, bool IsCypher);
    void MakeStr(QString str, QString *strToMake, uint64_t str_len, bool IsRus);
    void fileWrite(QString str);
    void FullText(QString encryption, QString *text, bool IsStolb);
    QString Stolb_encryption(int tk_length, int tt_length, QString true_text, int arr_ind[]);
    QString Stolb_decryption(int tk_length, int tt_length, QString true_text, int arr_ind[]);
    QString Vigener_encryption(QString new_key, QString true_text, int tt_length);
    QString Vigener_decryption(QString new_key, QString true_text, int tt_length, int tk_length);
    bool Perform_Method(uint64_t text_len, uint64_t key_len, QString textSt, QString keySt, QString *encryptionSt, bool IsCypher, bool IsStolb);

    void on_pushButton_Cypher_clicked();

    void on_pushButton_Cypher_f_clicked();

    void on_pushButton_Decypher_clicked();

    void on_pushButton_Decypher_f_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
