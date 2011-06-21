
#include "codecoptimizations.h"

#include <KLocale>
#include <KIcon>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QButtonGroup>
#include <KPushButton>


CodecOptimizations::CodecOptimizations( const QList<Optimization>& _optimizationList, QWidget* parent, Qt::WFlags f )
    : KDialog( parent, f ),
    optimizationList( _optimizationList )
{
    setCaption( i18n("Solutions for backend problems") );
    setWindowIcon( KIcon("help-about") );
    setButtons( KDialog::Ok | KDialog::Cancel );
    setButtonFocus( KDialog::Cancel );
    connect( this, SIGNAL(okClicked()), this, SLOT(okClicked()) );

    QWidget *widget = new QWidget( this );
    setMainWidget( widget );
    QVBoxLayout *box = new QVBoxLayout( widget );

    QLabel *messageLabel = new QLabel( i18n("You have installed or removed backends and your soundKonverter settings can be optimized."), this );
    box->addWidget( messageLabel );

    QFrame *frame = new QFrame( widget );
    frame->setFrameShape( QFrame::StyledPanel );
    frame->setFrameShadow( QFrame::Sunken );
    box->addWidget( frame );

    QGridLayout *grid = new QGridLayout( frame );
    grid->setColumnStretch( 1, 0 );
    grid->setColumnStretch( 2, 0 );

    for( int i=0; i<optimizationList.count(); i++ )
    {
        const QString codecName = optimizationList.at(i).codecName;
        const Optimization::Mode mode = optimizationList.at(i).mode;
        const QString currentBackend = optimizationList.at(i).currentBackend;
        const QString betterBackend = optimizationList.at(i).betterBackend;

        QLabel *solutionLabel = new QLabel( frame );
        grid->addWidget( solutionLabel, i, 0 );
        if( mode == Optimization::Encode )
            solutionLabel->setText( i18n( "For encoding %1 files the backend '%2' can be replaced with '%3'.", codecName, currentBackend, betterBackend ) );
        else if( mode == Optimization::Decode )
            solutionLabel->setText( i18n( "For decoding %1 files the backend '%2' can be replaced with '%3'.", codecName, currentBackend, betterBackend ) );
        else if( mode == Optimization::ReplayGain )
            solutionLabel->setText( i18n( "For applying Replay Gain to %1 files the backend '%2' can be replaced with '%3'.", codecName, currentBackend, betterBackend ) );

        KPushButton *solutionIgnore = new KPushButton( KIcon("dialog-cancel"), i18n("Ignore"), frame );
        solutionIgnore->setCheckable( true );
        grid->addWidget( solutionIgnore, i, 1 );

        KPushButton *solutionFix = new KPushButton( KIcon("dialog-ok-apply"), i18n("Fix"), frame );
        solutionFix->setCheckable( true );
        solutionFix->setChecked( true );
        solutionFixButtons.append( solutionFix );
        grid->addWidget( solutionFix, i, 2 );

        QButtonGroup *solutionButtonGroup = new QButtonGroup( frame );
        solutionButtonGroup->addButton( solutionIgnore );
        solutionButtonGroup->addButton( solutionFix );
    }
}

CodecOptimizations::~CodecOptimizations()
{}

void CodecOptimizations::okClicked()
{
    for( int i=0; i<optimizationList.count(); i++ )
    {
        if( solutionFixButtons.at(i) && solutionFixButtons.at(i)->isChecked() )
            optimizationList[i].solution = Optimization::Fix;
        else
            optimizationList[i].solution = Optimization::Ignore;
    }

    emit solutions( optimizationList );
}
