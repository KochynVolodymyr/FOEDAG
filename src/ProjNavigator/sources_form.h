#ifndef SOURCES_FORM_H
#define SOURCES_FORM_H
#include <QAction>
#include <QTreeWidget>
#include <QWidget>

#include "NewProject/ProjectManager/project_manager.h"
#include "add_file_dialog.h"

#define SRC_TREE_DESIGN_TOP_ITEM "destopitem"
#define SRC_TREE_CONSTR_TOP_ITEM "constrtopitem"
#define SRC_TREE_SIM_TOP_ITEM "simtopitem"
#define SRC_TREE_DESIGN_SET_ITEM "desfilesetitem"
#define SRC_TREE_DESIGN_FILE_ITEM "desfileitem"
#define SRC_TREE_CONSTR_SET_ITEM "constrfilesetitem"
#define SRC_TREE_CONSTR_FILE_ITEM "constrfileitem"
#define SRC_TREE_SIM_SET_ITEM "simfilesetitem"
#define SRC_TREE_SIM_FILE_ITEM "simfileitem"

#define SRC_TREE_FLG_ACTIVE tr(" (Active)")
#define SRC_TREE_FLG_TOP tr(" (Top)")
#define SRC_TREE_FLG_TARGET tr(" (Target)")

namespace Ui {
class SourcesForm;
}

namespace FOEDAG {

class TclCommandIntegration;
class SourcesForm : public QWidget {
  Q_OBJECT
  friend class TclCommandIntegration;

 public:
  explicit SourcesForm(QWidget* parent = nullptr);
  ~SourcesForm();

  void InitSourcesForm(const QString& strFile);
  TclCommandIntegration* createTclCommandIntegarion();

  /*for test*/
  void TestOpenProject(int argc, const char* argv[]);

 signals:
  void OpenFile(QString);

 private slots:
  void SlotItempressed(QTreeWidgetItem* item, int column);
  void SlotItemDoubleClicked(QTreeWidgetItem* item, int column);

  void SetCurrentFileItem(const QString& strFileName);
  void SlotRefreshSourceTree();
  void SlotAddFile();
  void SlotOpenFile();
  void SlotRemoveFile();
  void SlotSetAsTop();
  void SlotSetAsTarget();

 private:
  Ui::SourcesForm* ui;

  QTreeWidget* m_treeSrcHierachy;
  QAction* m_actRefresh;
  QAction* m_actAddFile;
  QAction* m_actOpenFile;
  QAction* m_actRemoveFile;
  QAction* m_actSetAsTop;
  QAction* m_actSetAsTarget;

  ProjectManager* m_projManager;

  void CreateActions();
  void UpdateSrcHierachyTree();
  void CreateFolderHierachyTree();
  static QTreeWidgetItem* CreateFolderHierachyTree(QTreeWidgetItem* topItem,
                                                   const QString& path);
  static QTreeWidgetItem* CreateParentFolderItem(QTreeWidgetItem* parent,
                                                 const QString& text);
  static QTreeWidgetItem* ChildByText(QTreeWidgetItem* topItem,
                                      const QString& text);
  static QString StripPath(const QString& path);
};
}  // namespace FOEDAG

#endif  // SOURCES_FORM_H
