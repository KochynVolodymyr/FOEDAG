#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QObject>
#include <QWidget>

#include "text_editor_form.h"

namespace FOEDAG {

class TextEditor : public QWidget {
  Q_OBJECT
 public:
  explicit TextEditor(QWidget *parent = nullptr);

  void ShowTextEditor();
  void ClosetextEditor();
  QWidget *GetTextEditor();

 signals:
  void CurrentFileChanged(QString);

 public slots:
  void SlotOpenFile(const QString &strFileName);

 private slots:
  void SlotCurrentFileChanged(const QString &strFileName);
};
}  // namespace FOEDAG
#endif  // TEXTEDITOR_H