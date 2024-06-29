#ifndef PROMOTIONDIALOG_H
#define PROMOTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(bool isWhite, QWidget *parent = nullptr);

    // Returns the selected piece
    uint8_t selectedPieceType() const;

private slots:
    void queenSelected();
    void rookSelected();
    void bishopSelected();
    void knightSelected();

private:
    uint8_t m_selectedPieceType;
    QPushButton* createPieceButton(const QString &imagePath, const QString &tooltip);
};

#endif
