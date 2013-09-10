/****************************************************************************
**
** Copyright (C) 2012 Donna Whisnant, a.k.a. Dewtronics.
** Contact: http://www.dewtronics.com/
**
** This file is part of the KJVCanOpener Application as originally written
** and developed for Bethel Church, Festus, MO.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU General Public License
** version 3.0 as published by the Free Software Foundation and appearing
** in the file gpl-3.0.txt included in the packaging of this file. Please
** review the following information to ensure the GNU General Public License
** version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and
** Dewtronics.
**
****************************************************************************/

#ifndef KJVSEARCHRESULT_H
#define KJVSEARCHRESULT_H

#include "dbstruct.h"
#include "UserNotesDatabase.h"
#include "SearchPhraseListModel.h"
#include "VerseListModel.h"

#include <QWidget>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QString>
#include <QLabel>
#include <QTreeView>
#include <QColor>
#include <QStyleOptionViewItem>

#include <assert.h>

// ============================================================================

// Forward declarations:
class CKJVCanOpener;
class CReflowDelegate;

class CNoteKeywordWidget;

// ============================================================================

 class CSearchResultsTreeView : public QTreeView
{
	Q_OBJECT
public:
	explicit CSearchResultsTreeView(CBibleDatabasePtr pBibleDatabase, CUserNotesDatabasePtr pUserNotesDatabase, QWidget *parent = 0);
	virtual ~CSearchResultsTreeView();

	inline QMenu *getEditMenu() { return m_pEditMenu; }
	inline QMenu *getLocalEditMenu() { return m_pEditMenuLocal; }
	inline QAction *getLocalEditMenuInsertionPoint() const { return m_pMenuInsertionPoint; }

	bool haveDetails() const;
	bool isActive() const;

	inline CVerseListModel *vlmodel() const {
		assert(model() != NULL);
		return static_cast<CVerseListModel *>(model());
	}

	inline CVerseListModel::VERSE_VIEW_MODE_ENUM viewMode() const { return vlmodel()->viewMode(); }
	inline CVerseListModel::VERSE_DISPLAY_MODE_ENUM displayMode() const { return vlmodel()->displayMode(); }
	inline CVerseListModel::VERSE_TREE_MODE_ENUM treeMode() const { return vlmodel()->treeMode(); }
	inline bool showMissingLeafs() const { return vlmodel()->showMissingLeafs(); }
	inline CRelIndex singleCrossRefSourceIndex() const { return vlmodel()->singleCrossRefSourceIndex(); }

	TVerseIndex currentVerseIndex() const;
	bool editableNodeSelected() const;

	QModelIndexList getSelectedVerses() const;

	CKJVCanOpener *parentCanOpener() const;

protected slots:
	void en_findParentCanOpener();

	void en_copyVerseText() const;
	void en_copyRaw() const;
	void en_copyVeryRaw() const;
	void en_copyVerseHeadings() const;
	void en_copyReferenceDetails() const;
	void en_copyComplete() const;

	void en_listChanged();

public slots:
	virtual bool setCurrentIndex(const TVerseIndex &ndx, bool bFocusTreeView = true);
	virtual void setViewMode(CVerseListModel::VERSE_VIEW_MODE_ENUM nViewMode);
	virtual void setDisplayMode(CVerseListModel::VERSE_DISPLAY_MODE_ENUM nDisplayMode);
	virtual void setTreeMode(CVerseListModel::VERSE_TREE_MODE_ENUM nTreeMode);
	virtual void setShowMissingLeafs(bool bShowMissing);
	virtual void setSingleCrossRefSourceIndex(const CRelIndex &ndx);
	virtual void setParsedPhrases(const CSearchCriteria &aSearchCriteria, const TParsedPhrasesList &phrases);		// Will build verseList and return the list of tags so they can be passed to a highlighter, etc

	virtual void showPassageNavigator();
	virtual void showDetails();
	virtual void setFontSearchResults(const QFont& aFont);
	virtual void setTextBrightness(bool bInvert, int nBrightness);

signals:
	void activatedSearchResults();
	void gotoIndex(const TPhraseTag &tag);
	void canExpandAll(bool bEnable);
	void canCollapseAll(bool bEnable);
	void currentItemChanged();
	void selectionListChanged();

protected:
	virtual void focusInEvent(QFocusEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	virtual void selectionChanged (const QItemSelection &selected, const QItemSelection &deselected);

	void copyRawCommon(bool bVeryRaw) const;
	void handle_selectionChanged();

	virtual void resizeEvent(QResizeEvent *event);

	virtual QStyleOptionViewItem viewOptions() const;

// Private Data:
private:

// Private UI:
private:
	bool m_bInvertTextBrightness;	// Local copies so we can have different current values than the app setting so we can preview settings
	int m_nTextBrightness;
	// ----
	bool m_bDoingPopup;				// True if popping up a menu or dialog and we don't want the highlight to disable
	QMenu *m_pEditMenu;				// Edit menu for main screen when this editor is active
	QMenu *m_pEditMenuLocal;		// Edit menu for local popup when user right-clicks -- like above but includes view toggles
	// ----
	QAction *m_pActionCopyVerseText;			// Edit menu copy text
	QAction *m_pActionCopyRaw;		// Edit menu copy raw phrase text
	QAction *m_pActionCopyVeryRaw;	// Edit menu copy very (no punctuation) raw phrase text
	// ----
	QAction *m_pActionCopyVerseHeadings;		// Edit menu copy headings
	QAction *m_pActionCopyReferenceDetails;		// Edit menu Reference ToolTip Copy
	QAction *m_pActionCopyComplete;				// Edit menu copy everything
	// ----
	QAction *m_pActionSelectAll;	// Edit menu select all
	QAction *m_pActionClearSelection;	// Edit menu clear selection
	// ----
	QAction *m_pMenuInsertionPoint;				// Point in the local menu for the main KJVCanOpener to insert the view menu
	QAction *m_pMenuUserNotesInsertionPoint;	// Point in the local menu for us to insert the user notes items once the parent CKJVCanOpener is discovered
	// ----
	QAction *m_pActionNavigator;	// Launch Passage Navigator for Search Result
	// ----
	QAction *m_pStatusAction;		// Used to update the status bar without an enter/leave sequence
	// ----
	CReflowDelegate *m_pReflowDelegate;
	// ----
	mutable CKJVCanOpener *m_pParentCanOpener;	// Parent CanOpener once we locate it.  Set lazily on demand since parent doesn't exist yet during object creation
};

// ============================================================================

class CKJVSearchResult : public QWidget
{
	Q_OBJECT
	
public:
	CKJVSearchResult(CBibleDatabasePtr pBibleDatabase, QWidget *parent = 0);
	~CKJVSearchResult();
	
	inline CVerseListModel::VERSE_VIEW_MODE_ENUM viewMode() const { return m_pSearchResultsTreeView->viewMode(); }
	inline CVerseListModel::VERSE_DISPLAY_MODE_ENUM displayMode() const { return m_pSearchResultsTreeView->displayMode(); }
	inline CVerseListModel::VERSE_TREE_MODE_ENUM treeMode() const { return m_pSearchResultsTreeView->treeMode(); }
	inline bool showMissingLeafs() const { return m_pSearchResultsTreeView->showMissingLeafs(); }
	inline CRelIndex singleCrossRefSourceIndex() const { return m_pSearchResultsTreeView->singleCrossRefSourceIndex(); }

	QModelIndex currentIndex() const;
	TVerseIndex currentVerseIndex() const;
	bool hasFocusSearchResult() const;
	bool editableNodeSelected() const;

	inline QMenu *getEditMenu() { return m_pSearchResultsTreeView->getEditMenu(); }
	inline QMenu *getLocalEditMenu() { return m_pSearchResultsTreeView->getLocalEditMenu(); }
	inline QAction *getLocalEditMenuInsertionPoint() const { return m_pSearchResultsTreeView->getLocalEditMenuInsertionPoint(); }

	inline bool haveDetails() const { return m_pSearchResultsTreeView->haveDetails(); }
	inline bool isActive() const { return m_pSearchResultsTreeView->isActive(); }

	inline bool haveResults() const { return (vlmodel()->searchResults().GetResultsCount() > 0); }

	QString searchResultsSummaryText() const;

public slots:
	bool setCurrentIndex(const TVerseIndex &ndx, bool bFocusTreeView = true);
	void setFocusSearchResult();
	void setViewMode(CVerseListModel::VERSE_VIEW_MODE_ENUM nViewMode);
	void setDisplayMode(CVerseListModel::VERSE_DISPLAY_MODE_ENUM nDisplayMode);
	void setTreeMode(CVerseListModel::VERSE_TREE_MODE_ENUM nTreeMode);
	void setShowMissingLeafs(bool bShowMissing);
	void setSingleCrossRefSourceIndex(const CRelIndex &ndx);
	void showPassageNavigator();
	void showDetails();
	void setParsedPhrases(const CSearchCriteria &aSearchCriteria, const TParsedPhrasesList &phrases);		// Will build verseList and return the list of tags so they can be passed to a highlighter, etc
	void keywordListChanged(bool bInitialLoad = false);

signals:			// Outgoing Pass-Through:
	void activated(const QModelIndex &);
	void gotoIndex(const TPhraseTag &);
	void changedSearchResults();
	void setDetailsEnable();

	void activatedSearchResults();
	void canExpandAll(bool bEnable);
	void canCollapseAll(bool bEnable);
	void currentItemChanged();

signals:			// Incoming Pass-Through:
	void expandAll();
	void collapseAll();
	void setFontSearchResults(const QFont &aFont);
	void setTextBrightness(bool bInvert, int nBrightness);

public:
	inline CVerseListModel *vlmodel() const { return m_pSearchResultsTreeView->vlmodel(); }

private:
	void setSearchResultsType();

private slots:
	void en_modelKeywordListChanged();

// Private Data:
private:
	CBibleDatabasePtr m_pBibleDatabase;
	int m_nLastSearchOccurrences;		// Last search summary of 'n' occurrences in 'x' verses in 'y' chapters in 'z' books
	int m_nLastSearchVerses;
	int m_nLastSearchChapters;
	int m_nLastSearchBooks;
	bool m_bLastCalcSuccess;
	int m_nLastSearchNumPhrases;
	CSearchCriteria m_LastSearchCriteria;

// UI Private:
private:
	bool m_bDoingUpdate;
	QLabel *m_pSearchResultsType;
	QLabel *m_pSearchResultsCount;
	CNoteKeywordWidget *m_pNoteKeywordWidget;
	CSearchResultsTreeView *m_pSearchResultsTreeView;
};

#endif // KJVSEARCHRESULT_H
