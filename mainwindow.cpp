#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QTextEdit"
#include <stdio.h>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Настройка формы
    int formWidth = 770;
    int formHeight = 750;
    this->setFixedSize(formWidth, formHeight);
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();
    this->move((screenWidth - formWidth) / 2, (screenHeight - formHeight) / 2);

    ui->radioButton_Stolb_method->setChecked(true);
    ui->radioButton_Shifr->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
FILE *file;
void MainWindow::MakeStr(QString str, QString *strToMake, uint64_t str_len, bool IsRus) {
    for (int i = 0; i < str_len; i++)
        if (IsValidSymbol(str[i], IsRus))
            (*strToMake) += str[i];
}

int MainWindow::IsValidSymbol(QChar symbol, bool IsRus) {
    if (!IsRus) {
        for (int i = 65; i < 91; i++)
            if (QChar(i) == symbol)
                return i;
        for (int i = 97; i < 123; i++)
            if (QChar(i) == symbol)
                return i;
    }
    else {
        for (int i = 1040; i < 1104; i++)
            if (QChar(i) == symbol)
                return i;
        if (QChar(1025) == symbol)
            return 1025;
        if (QChar(1105) == symbol)
            return 1105;
    }
    return 0;
}

bool MainWindow::IsUsed(int arr[], int ind, int len) {
    int i = 0;
    while (i < len && arr[i] != -1) {
        if (ind == arr[i])
            return true;
        i++;
    }
    return false;
}

int MainWindow::findMin(QString str, int arr[], int len) {
    if (len > 0) {
        int min = -1;
        QChar min_ch = QChar(1200);
        for (int i = len - 1; i >= 0; i--)
            if (!IsUsed(arr, i, len))
                if (min_ch >= str[i]) {
                    min_ch = str[i];
                    min = i;
                }
        return min;
    }
    return -1;
}

void MainWindow::FullText(QString encryption, QString *text, bool IsStolb) {
    int j = 0;
    int tt_length = encryption.length();
    uint64_t text_len = (*text).length();
    for (int i = 0; i < text_len; i++) {
        if (IsValidSymbol((*text)[i], !IsStolb)) {
            if (j < tt_length)
                (*text)[i] = encryption[j];
            j++;
        };
    }
}

void MainWindow::fileWrite(QString str) {
    QByteArray byteArray = str.toUtf8();
    const char *text_st_f_char = byteArray.constData();
    file = fopen("TextFile.txt", "w");
    fprintf(file, "%s", text_st_f_char);
    fclose(file);
}

QString MainWindow::Vigener_decryption(QString new_key, QString true_text, int tt_length, int tk_length) {
    QString ru_alphabet = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    QString decryption = "";
    for (int i = 0; i < tt_length; i++) {
        if (i % tk_length == 0 && i != 0)
            for (int j = 0; (j < tk_length) && (j + i < tt_length); j++) {
                new_key[i + j] = decryption[i + j - tk_length].toUpper();
            }
        int text_ind = 0;
        int key_ind = 0;
        QChar tt_up;
        if (true_text[i].isLower())
            tt_up = true_text[i].toUpper();
        else
            tt_up = true_text[i];
        while (ru_alphabet[text_ind] != tt_up || ru_alphabet[key_ind] != new_key[i]) {
            if (ru_alphabet[text_ind] != tt_up)
                text_ind++;
            if (ru_alphabet[key_ind] != new_key[i])
                key_ind++;
        }
        if (true_text[i].isUpper())
            decryption += ru_alphabet[(text_ind - key_ind + 33) % 33];
        else {
            if (ui->radioButton_Save_reg->isChecked())
                decryption += ru_alphabet[(text_ind - key_ind + 33) % 33].toLower();
            else
                decryption += ru_alphabet[(text_ind - key_ind + 33) % 33];
        }
    }
    return decryption;
}

QString MainWindow::Vigener_encryption(QString new_key, QString true_text, int tt_length) {
    QString ru_alphabet = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    QString encryption = "";
    for (int i = 0; i < tt_length; i++) {
        int text_ind = 0;
        int key_ind = 0;
        QChar tt_up;
        if (true_text[i].isLower())
            tt_up = true_text[i].toUpper();
        else
            tt_up = true_text[i];
        while (ru_alphabet[text_ind] != tt_up || ru_alphabet[key_ind] != new_key[i]) {
            if (ru_alphabet[text_ind] != tt_up)
                text_ind++;
            if (ru_alphabet[key_ind] != new_key[i])
                key_ind++;
        }
        if (true_text[i].isUpper())
            encryption += ru_alphabet[(key_ind + text_ind) % 33];
        else {
            if (ui->radioButton_Save_reg->isChecked())
                encryption += ru_alphabet[(key_ind + text_ind) % 33].toLower();
            else
                encryption += ru_alphabet[(key_ind + text_ind) % 33];
        }
    }
    return encryption;
}

QString MainWindow::Vigener_SelfGen(int tt_length, int tk_length, QString true_text, QString true_key, bool IsCypher){
    QString new_key = "";
    int i = 0;
    while (i < tt_length) {
        int j = 0;
        while (j < tk_length && i < tt_length) {
            new_key += true_key[j];
            j++;
            i++;
        }
        j = 0;
        while (j < tt_length && i < tt_length) {
            new_key += true_text[j].toUpper();
            j++;
            i++;
        }

    }
    if (IsCypher)
        return Vigener_encryption(new_key, true_text, tt_length);
    else
        return Vigener_decryption(new_key, true_text, tt_length, tk_length);
}

QString MainWindow::Stolb_decryption(int tk_length, int tt_length, QString true_text, int arr_ind[]) {
    QString decryption = "";
    int row_num[tk_length];
    for (int i = 0; i < tk_length; i++)
        row_num[i] = 0;
    for (int ii = 0; ii < tk_length; ii++) {
        int substr_size = 0;
        int ind = 0;
        int i = -1;
        int active_size = arr_ind[ii] + 1;
        while (i < tt_length) {
            substr_size = arr_ind[ind % tk_length] + 1;
            if (active_size <= substr_size && i + active_size < tt_length) {
                row_num[ii]++;
            }
            i += substr_size;
            ind++;
        }
    }
    for (int i = 0; i < tt_length; i++)
        decryption += ' ';
    for (int ii = 0; ii < tk_length; ii++) {
        int substr_size = 0;
        int active_size = arr_ind[ii] + 1;
        int j = -1;
        int i = 0;
        int ind = 0;
        while (i < row_num[ii]) {
            substr_size = arr_ind[ind % tk_length] + 1;
            if (active_size <= substr_size) {
                j += substr_size - (substr_size - active_size);
                if (j < tt_length) {
                    int index = 0;
                    for (int k = 0; k < ii; k++)
                        index += row_num[k];
                    if (ii > 0)
                        index += i;
                    else
                        index = i;
                    if (index < tt_length)
                        decryption[j] = true_text[index];
                    j += substr_size - active_size;
                    i++;
                }
            }
            else
                j += substr_size;
            ind++;
        }
    }
    return decryption;
}

QString MainWindow::Stolb_encryption(int tk_length, int tt_length, QString true_text, int arr_ind[]) {
    QString encryption = "";
    for (int ii = 0; ii < tk_length; ii++) {
        int substr_size = 0;
        int ind = 0;
        int i = -1;
        int active_size = arr_ind[ii] + 1;
        while (i < tt_length) {
            substr_size = arr_ind[ind % tk_length] + 1;
            if (active_size <= substr_size) {
                i += substr_size - (substr_size - active_size);
                if (i < tt_length) {
                    encryption += true_text[i];
                    i += substr_size - active_size;
                }
            }
            else
                i += substr_size;
            ind++;
        }
    }
    return encryption;
}

QString MainWindow::Column_Improved(int tt_length, int tk_length, QString true_text, QString true_key, bool IsCypher){
    int arr_ind[tk_length];
    for (int i = 0; i < tk_length; i++)
        arr_ind[i] = -1;
    for (int i = 0; i < tk_length; i++) {
        int min = findMin(true_key, arr_ind, tk_length);
        arr_ind[i] = min;
    }
    if (IsCypher)
        return Stolb_encryption(tk_length, tt_length, true_text, arr_ind);
    else
        return Stolb_decryption(tk_length, tt_length, true_text, arr_ind);
}

bool MainWindow::Perform_Method(uint64_t text_len, uint64_t key_len, QString textSt, QString keySt, QString *encryptionSt, bool IsCypher, bool IsStolb){
    QString true_text = "", true_key = "";
    MakeStr(textSt, &true_text, text_len, !IsStolb);
    MakeStr(keySt, &true_key, key_len, !IsStolb);
    int tt_length = true_text.length();
    int tk_length = true_key.length();
    true_key = true_key.toUpper();
    if (tk_length != 0 && tt_length != 0) {
        if (IsStolb) {
            (*encryptionSt) = Column_Improved(tt_length, tk_length, true_text, true_key, IsCypher);
            if (!ui->radioButton_Save_reg->isChecked())
                (*encryptionSt) = (*encryptionSt).toUpper();
        }
        else
            (*encryptionSt) = Vigener_SelfGen(tt_length, tk_length, true_text, true_key, IsCypher);
        return true;
    }
    return false;
}

void MainWindow::on_pushButton_Cypher_clicked()
{
    uint64_t text_len = ui->textEdit->toPlainText().length();
    uint64_t key_len = ui->textEdit_Key->toPlainText().length();
    if (key_len != 0 && text_len != 0) {
        QString  text = ui->textEdit->toPlainText();
        QString key = ui->textEdit_Key->toPlainText();
        QString encryption = "";
        if (Perform_Method(text_len, key_len, text, key, &encryption, true, ui->radioButton_Stolb_method->isChecked())) {
            if (ui->radioButton_Shifr->isChecked())
                ui->textEdit_Cypher->setText(encryption);
            else {
                FullText(encryption, &text, ui->radioButton_Stolb_method->isChecked());
                ui->textEdit_Cypher->setText(text);
            }
        }
        else
            ui->textEdit_Cypher->setText("");
    }
}

void MainWindow::on_pushButton_Cypher_f_clicked()
{
    QString text_f = "";
    QString encryption_f = "";
    QString key_f = "";
    file = fopen("TextFile.txt", "r");
    char str[2048];
    while (fgets(str, 2048, file) != NULL)
        text_f += str;
    fclose(file);

    key_f = ui->textEdit_Key->toPlainText();
    uint64_t text_len = text_f.length();
    uint64_t key_len = key_f.length();

    if (key_len != 0 && text_len != 0) {
        if (Perform_Method(text_len, key_len, text_f, key_f, &encryption_f, true, ui->radioButton_Stolb_method->isChecked())) {
            if (!ui->radioButton_All_Symbols->isChecked())
                fileWrite(encryption_f);
            else {
                FullText(encryption_f, &text_f, ui->radioButton_Stolb_method->isChecked());
                fileWrite(text_f);
            }
        }
    }
}

void MainWindow::on_pushButton_Decypher_clicked()
{   uint64_t text_len = ui->textEdit_Cypher->toPlainText().length();
    uint64_t key_len = ui->textEdit_Key->toPlainText().length();
    if (key_len != 0 && text_len != 0) {
        QString text = ui->textEdit_Cypher->toPlainText();
        QString key = ui->textEdit_Key->toPlainText();
        QString decryption = "";
        if (Perform_Method(text_len, key_len, text, key, &decryption, false, ui->radioButton_Stolb_method->isChecked())) {
            FullText(decryption, &text, ui->radioButton_Stolb_method->isChecked());
            ui->textEdit_Decypher->setText(text);
        }
        else
            ui->textEdit_Decypher->setText("");
    }
}

void MainWindow::on_pushButton_Decypher_f_clicked()
{
    QString text_f = "";
    QString decryption_f = "";
    QString key_f = "";
    file = fopen("TextFile.txt", "r");
    char str[2048];
    while (fgets(str, 2048, file) != NULL) {
        text_f += str;
    }
    fclose(file);

    key_f = ui->textEdit_Key->toPlainText();
    uint64_t text_len = text_f.length();
    uint64_t key_len = key_f.length();

    if (key_len != 0 && text_len != 0) {
        if (Perform_Method(text_len, key_len, text_f, key_f, &decryption_f, false, ui->radioButton_Stolb_method->isChecked())) {
            FullText(decryption_f, &text_f, ui->radioButton_Stolb_method->isChecked());
            fileWrite(text_f);
        }
    }
}
/*
    int l_gramm_length = 3;
    int ind = 0;
    int inds_l_gramm[(int)(text_len / l_gramm_length)];
    for (int i = 0; i < text_len - l_gramm_length; i++) {
        QString l_gramm = "";
        for (int j = 0; j < l_gramm_length; j++) {
            l_gramm += true_text[j + i];
        }
        for (int k = i + l_gramm_length; k < text_len - l_gramm_length; k++) {
            QString temp_gramm = "";
            for (int ii = 0; ii < l_gramm_length; ii++) {
                temp_gramm += true_text[k + ii];
            }
            if (temp_gramm == l_gramm) {
                inds_l_gramm[ind++] = k;
            }
        }
    }
    */
/* QString asciiTable;
    for (int i = 97; i < 123; i++) {
        asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(i)).arg(i);
    }
    for (int i = 65; i < 91; i++) {
        asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(i)).arg(i);
    }
    for (int i = 1072; i < 1104; i++) {
        asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(i)).arg(i);
        if (i == 1077) {
            asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(1105)).arg(1105);
        }
    }
    for (int i = 1040; i < 1072; i++) {
        asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(i)).arg(i);
        if (i == 1045) {
            asciiTable += QString("Символ: %1\tКод: %2\n").arg(QChar(1025)).arg(1025);
        }
    }
    ui->textEdit_Stolb_Cypher->setText(asciiTable);   */
/*
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc nec nisi ultricies, hendrerit ex in, vestibulum lectus.
Quisque eu felis purus.
Sed sit amet ultrices risus.
Vivamus non erat vel libero ultricies scelerisque.
Vestibulum at dolor quis orci rhoncus interdum.
Etiam nec tortor ut justo aliquet vehicula.
Mauris vitae enim sit amet enim interdum ultricies.
In at libero sed nunc consequat ultricies.
Sed ac tristique libero, nec lobortis felis.
Fusce congue metus nec lacinia fermentum.
Cras eget suscipit elit. Integer scelerisque vel eros nec hendrerit.
Phasellus vel sem eu lectus malesuada dignissim.
Nam vestibulum, mauris nec euismod faucibus, sapien nulla finibus felis, nec bibendum ligula justo non arcu.
Duis sed risus vel libero ultrices convallis.Mauris sit amet neque ut ex tristique ultricies.
*/
/*for (int ii = 0; ii < tk_length; ii++) { Information theory
                int substr_size = 0;
                int ind = 0;
                int i = -1;
                int active_size = arr_ind[ii] + 1;
                while (i < tt_length) {
                    substr_size = arr_ind[ind % tk_length] + 1;
                    i += substr_size;
                    if (i < tt_length) {
                        (*encryptionSt) += true_text[i];
                    }
                    ind++;
                }
            }*/
/*int row_num[tk_length];
            for (int i = 0; i < tk_length; i++) {
                row_num[i] = 0;
            }
            int len = 0;
            int Rows = 0;
            int ind = 0;
            while (len < tt_length) {
                Rows++;
                len += (arr_ind[ind % tk_length]) + 1;
                row_num[ind % tk_length]++;
                ind++;
            }
            int substr_size = 0;*/

/*while (substr_size < tt_length){
                substr_size = arr_ind[ind] + 1;
                for (int i = 0; i < arr_ind[ind]; i++) {
                    row_num[i]++;
                }
            }*/

//Information theory
/*int len = 0;
        int min = 0;
        int Rows = 0;
        int ind = 0;
        while (len < tt_length) {
            Rows++;
            len += (arr_ind[ind % tk_length]) + 1;
            ind++;
        }

        QString matrix[Rows][tk_length];
        ind = 0;
        int i = 0;
        while (i < tt_length) {
            for (int j = 0; j <= arr_ind[ind % tk_length] && i < tt_length; j++)
                matrix[ind][j] = true_text[i++];
            ind++;
        }
        for (ind = 0; ind < tk_length; ind++)
            for (int j = 0; j < Rows; j++)
                if (matrix[j][arr_ind[ind]] != "")
                    (*encryptionSt) += matrix[j][arr_ind[ind]]; */

