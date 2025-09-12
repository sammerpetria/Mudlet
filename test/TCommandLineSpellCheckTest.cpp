#include <QAccessible>
#include <QPlainTextEdit>
#include <QTextCodec>
#include <QtTest/QtTest>

struct Hunhandle {};

int Hunspell_spell(Hunhandle *, const char *word) {
  return strcmp(word, "helo") != 0;
}

class DummyConsole {
public:
  Hunhandle *getHunspellHandle_system() { return &mHandle; }
  QTextCodec *getHunspellCodec_system() {
    return QTextCodec::codecForName("UTF-8");
  }
  Hunhandle *getHunspellHandle_user() { return nullptr; }

private:
  Hunhandle mHandle;
};

class TestHost {
public:
  bool mEnableSpellCheck = true;
  DummyConsole *mpConsole = nullptr;
};

class mudlet {
public:
  static mudlet *self() {
    static mudlet instance;
    return &instance;
  }

  void announce(const QString &text) { announcements << text; }

  int mMinLengthForSpellCheck = 3;
  QStringList announcements;
};

class SpellCheckTestEdit : public QPlainTextEdit {
public:
  explicit SpellCheckTestEdit(TestHost *host) : mpHost(host) {}

  void spellCheck() {
    QTextCursor c = textCursor();
    if (!mSpellChecking) {
      spellCheckWord(c);
    }
  }

  void spellCheckWord(QTextCursor &c) {
    if (!mpHost || !mpHost->mEnableSpellCheck) {
      return;
    }

    Hunhandle *systemDictionaryHandle =
        mpHost->mpConsole->getHunspellHandle_system();
    if (!systemDictionaryHandle) {
      return;
    }

    QTextCharFormat f;
    mSpellChecking = true;
    c.select(QTextCursor::WordUnderCursor);

    if (!c.atBlockEnd() && !c.atEnd()) {
      const QChar nextChar = document()->characterAt(c.position());
      if (!nextChar.isSpace() && !nextChar.isPunct()) {
        f.setFontUnderline(false);
        c.setCharFormat(f);
        setTextCursor(c);
        mSpellChecking = false;
        return;
      }
    }

    const QTextCharFormat oldFormat = c.charFormat();
    const QString spellCheckedWord = c.selectedText();
    const bool wantSpellCheck =
        spellCheckedWord.length() >= mudlet::self()->mMinLengthForSpellCheck;
    if (!wantSpellCheck) {
      f.setFontUnderline(false);
      c.setCharFormat(f);
      setTextCursor(c);
      mSpellChecking = false;
      return;
    }

    const QByteArray encodedText =
        mpHost->mpConsole->getHunspellCodec_system()->fromUnicode(
            spellCheckedWord);
    bool isMisspelled = false;
    if (!Hunspell_spell(systemDictionaryHandle, encodedText.constData())) {
      f.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
      f.setUnderlineColor(Qt::red);
      isMisspelled = true;
    } else {
      f.setFontUnderline(false);
    }

    if (isMisspelled && QAccessible::isActive() &&
        oldFormat.underlineStyle() != QTextCharFormat::SpellCheckUnderline) {
      mudlet::self()->announce(tr("spelling mistake"));
    }

    c.setCharFormat(f);
    setTextCursor(c);
    mSpellChecking = false;
  }

  TestHost *mpHost;
  bool mSpellChecking = false;
};

static void accessibilityUpdateHandler(QAccessibleEvent *) {}

class TCommandLineSpellCheckTest : public QObject {
  Q_OBJECT
private slots:
  void initTestCase() {
    qputenv("QT_ACCESSIBILITY", QByteArray("1"));
    QAccessible::installUpdateHandler(accessibilityUpdateHandler);
    mHost.mpConsole = &mConsole;
  }

  void testNoAnnouncementWhileTyping() {
    mudlet::self()->announcements.clear();
    SpellCheckTestEdit edit(&mHost);
    edit.setPlainText("helo");
    edit.moveCursor(QTextCursor::End);
    edit.spellCheck();
    QTextCursor c = edit.textCursor();
    c.select(QTextCursor::WordUnderCursor);
    QCOMPARE(c.charFormat().underlineStyle(), QTextCharFormat::NoUnderline);
    QCOMPARE(mudlet::self()->announcements.size(), 0);
  }

  void testAnnouncementAfterBoundary() {
    mudlet::self()->announcements.clear();
    SpellCheckTestEdit edit(&mHost);
    edit.setPlainText("helo ");
    edit.moveCursor(QTextCursor::End);
    edit.spellCheck();
    QTextCursor c = edit.textCursor();
    c.movePosition(QTextCursor::PreviousCharacter);
    c.select(QTextCursor::WordUnderCursor);
    QCOMPARE(c.charFormat().underlineStyle(),
             QTextCharFormat::SpellCheckUnderline);
    QCOMPARE(mudlet::self()->announcements.size(), 1);
    QCOMPARE(mudlet::self()->announcements.first(),
             QStringLiteral("spelling mistake"));
  }

private:
  TestHost mHost;
  DummyConsole mConsole;
};

QTEST_MAIN(TCommandLineSpellCheckTest)
#include "TCommandLineSpellCheckTest.moc"
