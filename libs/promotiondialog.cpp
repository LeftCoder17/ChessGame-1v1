#include "promotiondialog.h"
#include "piece.h"

PromotionDialog::PromotionDialog(bool isWhite, QWidget *parent)
    : QDialog(parent), m_selectedPieceType(queen) // Default to queen if no selection is made
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QString colorPrefix = isWhite ? "w" : "b";

    // Create buttons with images
    QPushButton *queenButton = createPieceButton(QString(":/libs/images/%1_queen.svg").arg(colorPrefix), "Queen");
    QPushButton *rookButton = createPieceButton(QString(":/libs/images/%1_rook.svg").arg(colorPrefix), "Rook");
    QPushButton *bishopButton = createPieceButton(QString(":/libs/images/%1_bishop.svg").arg(colorPrefix), "Bishop");
    QPushButton *knightButton = createPieceButton(QString(":/libs/images/%1_knight.svg").arg(colorPrefix), "Knight");

    connect(queenButton, &QPushButton::clicked, this, &PromotionDialog::queenSelected);
    connect(rookButton, &QPushButton::clicked, this, &PromotionDialog::rookSelected);
    connect(bishopButton, &QPushButton::clicked, this, &PromotionDialog::bishopSelected);
    connect(knightButton, &QPushButton::clicked, this, &PromotionDialog::knightSelected);

    buttonLayout->addWidget(queenButton);
    buttonLayout->addWidget(rookButton);
    buttonLayout->addWidget(bishopButton);
    buttonLayout->addWidget(knightButton);

    layout->addLayout(buttonLayout);
    setLayout(layout);
}

uint8_t PromotionDialog::selectedPieceType() const
{
    return m_selectedPieceType;
}

void PromotionDialog::queenSelected()
{
    m_selectedPieceType = queen;
    accept();
}

void PromotionDialog::rookSelected()
{
    m_selectedPieceType = rook;
    accept();
}

void PromotionDialog::bishopSelected()
{
    m_selectedPieceType = bishop;
    accept();
}

void PromotionDialog::knightSelected()
{
    m_selectedPieceType = knight;
    accept();
}

QPushButton* PromotionDialog::createPieceButton(const QString &imagePath, const QString &tooltip)
{
    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(imagePath));
    button->setIconSize(QSize(64, 64));
    button->setToolTip(tooltip);
    return button;
}
