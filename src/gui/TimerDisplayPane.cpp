//
// Created by Dylan Meadows on 2020-03-10.
//

#include "TimerDisplayPane.h"

#include <QFontDatabase>
#include <QGroupBox>
#include <QStyle>
#include <QVBoxLayout>

namespace gui {
    TimerDisplayPane::TimerDisplayPane(EonTimer::TimerService *timerService,
                                       const EonTimer::settings::ActionSettingsModel *actionSettings)
        : QGroupBox(nullptr), actionSettings(actionSettings) {
        currentStage = new QLabel("0:000");
        setVisualCue(actionSettings->getColor());
        connect(timerService, &EonTimer::TimerService::stateChanged, [this](const EonTimer::TimerState &state) {
            currentStage->setText(formatTime(state.remaining));
        });
        connect(actionSettings, &EonTimer::settings::ActionSettingsModel::colorChanged, [this](const QColor &color) {
            setVisualCue(color);
        });
        minutesBeforeTarget = new QLabel("0");
        connect(timerService,
                &EonTimer::TimerService::minutesBeforeTargetChanged,
                [this](const std::chrono::minutes &minutesBeforeTarget) {
                    this->minutesBeforeTarget->setText(QString::number(minutesBeforeTarget.count()));
                });
        nextStage = new QLabel("0:000");
        connect(
            timerService,
            &EonTimer::TimerService::nextStageChanged,
            [this](const std::chrono::milliseconds &nextStage) { this->nextStage->setText(formatTime(nextStage)); });
        connect(timerService, &EonTimer::TimerService::actionTriggered, this, &TimerDisplayPane::activate);
        connect(&timer, &QTimer::timeout, this, &TimerDisplayPane::deactivate);
        initComponents();
    }

    void TimerDisplayPane::initComponents() {
        // --- group ---
        {
            setProperty("class", "themeable-panel themeable-border");
            auto *rootLayout = new QVBoxLayout(this);
            rootLayout->setSpacing(5);
            // ----- currentStage ----
            {
                rootLayout->addWidget(currentStage);
                rootLayout->setAlignment(currentStage, Qt::AlignLeft);
                const int font = QFontDatabase::addApplicationFont(":/fonts/RobotoMono-Regular.ttf");
                const QString family = QFontDatabase::applicationFontFamilies(font)[0];
                currentStage->setObjectName("currentStageLbl");
                currentStage->setFont(QFont(family, 36));
            }
            // ----- minutesBeforeTarget -----
            {
                auto *layout = new QHBoxLayout();
                layout->setSpacing(5);
                rootLayout->addLayout(layout);
                rootLayout->setAlignment(layout, Qt::AlignLeft);
                layout->addWidget(new QLabel("Minutes Before Target:"));
                layout->addWidget(minutesBeforeTarget);
            }
            // ----- nextStage -----
            {
                auto *layout = new QHBoxLayout();
                layout->setSpacing(5);
                rootLayout->addLayout(layout);
                rootLayout->setAlignment(layout, Qt::AlignLeft);
                layout->addWidget(new QLabel("Next Stage:"));
                layout->addWidget(nextStage);
            }
        }
    }

    const QString TimerDisplayPane::formatTime(const std::chrono::milliseconds &milliseconds) const {
        const auto ms = milliseconds.count();
        if (ms > 0) {
            return QString::number(ms / 1000) + ":" + QString::number(ms % 1000).rightJustified(3, '0');
        } else if (ms < 0) {
            return "?:???";
        } else {
            return "0:000";
        }
    }

    void TimerDisplayPane::updateCurrentStageLbl() {
        currentStage->setProperty("active", isActive);
        QStyle *style = currentStage->style();
        style->unpolish(currentStage);
        style->polish(currentStage);
    }

    void TimerDisplayPane::setVisualCue(const QColor &color) {
        const QString style = "#currentStageLbl[active=\"true\"]{ background-color: rgb(%1, %2, %3); }";
        currentStage->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
    }

    bool TimerDisplayPane::isVisualCueEnabled() const {
        const auto mode = actionSettings->getMode();
        return mode == EonTimer::ActionMode::VISUAL || mode == EonTimer::ActionMode::AV;
    }

    void TimerDisplayPane::activate() {
        if (isVisualCueEnabled() && !isActive) {
            isActive = true;
            updateCurrentStageLbl();
            timer.start(75);
        }
    }

    void TimerDisplayPane::deactivate() {
        if (isVisualCueEnabled() && isActive) {
            isActive = false;
            updateCurrentStageLbl();
        }
    }

}  // namespace gui
