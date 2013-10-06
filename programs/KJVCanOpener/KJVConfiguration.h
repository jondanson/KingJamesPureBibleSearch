/****************************************************************************
**
** Copyright (C) 2013 Donna Whisnant, a.k.a. Dewtronics.
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

#ifndef KJVCONFIGURATION_H
#define KJVCONFIGURATION_H

#include "dbstruct.h"
#include "PhraseEdit.h"
#include "UserNotesDatabase.h"

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QwwConfigWidget>
#include <QFont>
#include <QPushButton>

// ============================================================================

// Forward Declarations:
class CSearchResultsTreeView;
class CScriptureBrowser;
class CScriptureEdit;
class CDictionaryWidget;
class QwwColorButton;
class CKJVTextFormatConfig;
class QListWidgetItem;

// ============================================================================


class CHighlighterColorButtonSignalReflector : public QObject
{
	Q_OBJECT

public:
	CHighlighterColorButtonSignalReflector(CKJVTextFormatConfig *pConfigurator, const QString &strUserDefinedHighlighterName);
	~CHighlighterColorButtonSignalReflector();

	QString highlighterName() const { return m_strUserDefinedHighlighterName; }

signals:
	void colorPicked(const QString &strUserDefinedHighlighterName, const QColor &color);
	void colorClicked(const QString &strUserDefinedHighlighterName);
	void enableChanged(const QString &strUserDefinedHighlighterName, bool bEnabled);

public slots:
	void en_colorPicked(const QColor &color);
	void en_clicked();
	void en_enableClicked(bool bEnabled);

protected slots:
	virtual void en_setTextBrightness(bool bInvert, int nBrightness) = 0;
	virtual void en_adjustDialogElementBrightnessChanged(bool bAdjust) = 0;

private:
	QString m_strUserDefinedHighlighterName;		// Name of User Defined Highlighter to use from persistent settings
};

// ============================================================================

#include "ui_KJVTextFormatConfig.h"

class CKJVTextFormatConfig : public QWidget
{
	Q_OBJECT

public:
	explicit CKJVTextFormatConfig(CBibleDatabasePtr pBibleDatabase, CDictionaryDatabasePtr pDictionary, QWidget *parent = 0);						// Databases for the preview
	~CKJVTextFormatConfig();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

public slots:
	void en_ScriptureBrowserFontChanged(const QFont &font);
	void en_SearchResultsFontChanged(const QFont &font);
	void en_DictionaryFontChanged(const QFont &font);
	void en_ScriptureBrowserFontSizeChanged(double nFontSize);
	void en_SearchResultsFontSizeChanged(double nFontSize);
	void en_DictionaryFontSizeChanged(double nFontSize);

	void en_InvertTextBrightnessChanged(bool bInvert);
	void en_TextBrightnessChanged(int nBrightness);
	void en_AdjustDialogElementBrightness(bool bAdjust);

	void en_WordsOfJesusColorPicked(const QColor &color);
	void en_SearchResultsColorPicked(const QColor &color);
	void en_CursorTrackerColorPicked(const QColor &color);
	void en_HighlighterColorPicked(const QString &strUserDefinedHighlighterName, const QColor &color);
	void en_HighlighterColorClicked(const QString &strUserDefinedHighlighterName);
	void en_HighlighterEnableChanged(const QString &strUserDefinedHighlighterName, bool bEnabled);

	void en_comboBoxHighlightersTextChanged(const QString &strUserDefinedHighlighterName);
	void en_addHighlighterClicked();
	void en_removeHighlighterClicked();
	void en_renameHighlighterClicked();
	void en_currentColorListViewItemChanged(QListWidgetItem *pCurrent, QListWidgetItem *pPrevious);

	void navigateToDemoText();
	void setPreview();

	void en_selectionChangedBrowser();

private:
	void recalcColorListWidth();

// Private Data:
private:
//	CBibleDatabasePtr m_pBibleDatabase;
//	CDictionaryDatabasePtr m_pDictionaryDatabase;
	CParsedPhrase m_previewSearchPhrase;		// Phrase for searching for preview
	QFont m_fntScriptureBrowser;
	QFont m_fntSearchResults;
	QFont m_fntDictionary;
	bool m_bInvertTextBrightness;
	int m_nTextBrightness;
	bool m_bAdjustDialogElementBrightness;

// UI Private:
private:
	CSearchResultsTreeView *m_pSearchResultsTreeView;
	CScriptureBrowser *m_pScriptureBrowser;
	CDictionaryWidget *m_pDictionaryWidget;
	bool m_bIsDirty;
	bool m_bLoadingData;

	static QwwColorButton *toQwwColorButton(QPushButton *pButton) { return reinterpret_cast<QwwColorButton *>(pButton); }
	Ui::CKJVTextFormatConfig ui;
};

// ============================================================================

#include "ui_KJVBibleDatabaseConfig.h"

class CKJVBibleDatabaseConfig : public QWidget
{
	Q_OBJECT

public:
	explicit CKJVBibleDatabaseConfig(CBibleDatabasePtr pBibleDatabase, QWidget *parent = 0);
	~CKJVBibleDatabaseConfig();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

// Data Private:
private:
	CBibleDatabasePtr m_pBibleDatabase;

// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;

	Ui::CKJVBibleDatabaseConfig ui;
};

// ============================================================================

#include "ui_KJVUserNotesDatabaseConfig.h"

class CKJVUserNotesDatabaseConfig : public QWidget
{
	Q_OBJECT

public:
	explicit CKJVUserNotesDatabaseConfig(CUserNotesDatabasePtr pUserNotesDatabase, QWidget *parent = 0);
	~CKJVUserNotesDatabaseConfig();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

private slots:
	void en_clickedSetPrimaryUserNotesFilename();
	void en_changedKeepBackup();
	void en_changedBackupExtension();

// Data Private:
private:
	CUserNotesDatabasePtr m_pUserNotesDatabase;

// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;

	Ui::CKJVUserNotesDatabaseConfig ui;
};

// ============================================================================

#include "ui_ConfigSearchOptions.h"

class CConfigSearchOptions : public QWidget
{
	Q_OBJECT

public:
	explicit CConfigSearchOptions(QWidget *parent = 0);
	~CConfigSearchOptions();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

private slots:
	void en_changedSearchPhraseCompleterFilterMode(int nIndex);
	void en_changedSearchPhraseActivationDelay(int nValue);

// Data Private:
private:


// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;

	Ui::CConfigSearchOptions ui;
};

// ============================================================================

#include "ui_ConfigBrowserOptions.h"

class CConfigBrowserOptions : public QWidget
{
	Q_OBJECT

public:
	explicit CConfigBrowserOptions(QWidget *parent = 0);
	~CConfigBrowserOptions();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

private slots:
	void en_changedNavigationActivationDelay(int nValue);
	void en_changedPassageReferenceActivationDelay(int nValue);

// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;

	Ui::CConfigBrowserOptions ui;
};

// ============================================================================

#include "ui_ConfigDictionaryOptions.h"

class CConfigDictionaryOptions : public QWidget
{
	Q_OBJECT

public:
	explicit CConfigDictionaryOptions(QWidget *parent = 0);
	~CConfigDictionaryOptions();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

private slots:
	void en_changedDictionaryCompleterFilterMode(int nIndex);
	void en_changedDictionaryActivationDelay(int nValue);

// Data Private:
private:


// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;

	Ui::CConfigDictionaryOptions ui;
};

// ============================================================================

#include "ui_ConfigCopyOptions.h"

class CConfigCopyOptions : public QWidget
{
	Q_OBJECT

public:
	explicit CConfigCopyOptions(QWidget *parent = 0);
	~CConfigCopyOptions();

	void initialize(CBibleDatabasePtr pBibleDatabase);						// Database for the preview

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const { return m_bIsDirty; }

signals:
	void dataChanged();

private slots:
	void en_changedReferenceDelimiterMode(int nIndex);
	void en_changedReferencesUseAbbreviatedBookNames(bool bUseAbbrBookName);
	void en_changedReferencesInBold(bool bInBold);
	void en_changedVerseNumberDelimiterMode(int nIndex);
	void en_changedVerseNumbersUseAbbreviatedBookNames(bool bUseAbbrBookName);
	void en_changedVerseNumbersInBold(bool bInBold);
	void en_changedAddQuotesAroundVerse(bool bAddQuotes);
	void en_changedTransChangeAddWordMode(int nIndex);

private:
	void setVerseCopyPreview();

// Data Private:
private:
	CBibleDatabasePtr m_pBibleDatabase;

// UI Private:
private:
	bool m_bIsDirty;
	bool m_bLoadingData;
	CScriptureEdit *m_pEditCopyOptionPreview;

	Ui::CConfigCopyOptions ui;
};

// ============================================================================

#include "ui_KJVGeneralSettingsConfig.h"

class CKJVGeneralSettingsConfig : public QWidget
{
	Q_OBJECT

public:
	explicit CKJVGeneralSettingsConfig(CBibleDatabasePtr pBibleDatabase, QWidget *parent = 0);
	~CKJVGeneralSettingsConfig();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system

	bool isDirty() const;

signals:
	void dataChanged();

// Data Private:
private:

// UI Private:
private:
	Ui::CKJVGeneralSettingsConfig ui;
};

// ============================================================================

class CKJVConfiguration : public QwwConfigWidget
{
	Q_OBJECT

public:
	CKJVConfiguration(CBibleDatabasePtr pBibleDatabase, CDictionaryDatabasePtr pDictionary, QWidget *parent = NULL);
	virtual ~CKJVConfiguration();

	void loadSettings();					// Reloads the settings (used for restore operation when abandoning changes)
	void saveSettings();					// Writes changes back to system
	bool isDirty() const;

signals:
	void dataChanged();

private:
	CKJVGeneralSettingsConfig *m_pGeneralSettingsConfig;
	CKJVTextFormatConfig *m_pTextFormatConfig;
	CKJVUserNotesDatabaseConfig *m_pUserNotesDatabaseConfig;
	CKJVBibleDatabaseConfig *m_pBibleDatabaseConfig;
};

// ============================================================================

class CKJVConfigurationDialog : public QDialog
{
	Q_OBJECT

public:
	CKJVConfigurationDialog(CBibleDatabasePtr pBibleDatabase, CDictionaryDatabasePtr pDictionary, QWidget *parent = NULL);
	virtual ~CKJVConfigurationDialog();

public slots:
	virtual void en_dataChanged();
	virtual void accept();
	virtual void reject();
	virtual void apply();
	virtual void restore(bool bRecopy);		// Restores setting changes (converse of apply).  If bRecopy=true, a fresh copy of the settings are made to continue edit.  If false, they are switched back to the original and left

private slots:
	void en_configurationIndexChanged(int index);
	void en_setToLastIndex();

private:
	int m_nLastIndex;						// Last Configuration Index active
	bool m_bHandlingPageSwap;				// Set to true while we are handling a page swap, used as a safe-guard in case we need to switch pages back
	CKJVConfiguration *m_pConfiguration;
	QDialogButtonBox *m_pButtonBox;
};

// ============================================================================

#endif	// KJVCONFIGURATION_H

