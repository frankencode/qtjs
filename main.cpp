#include <stdio.h>
#include <malloc.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <QCoreApplication>
#include <QScriptEngine>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QScriptEngine engine;
    while (true) {
        QString text;
        const char* prompt = "js> ";
        char* buf = 0;
        while (true) {
            buf = readline(prompt);
            if (!buf) break;
            QString line = QString::fromUtf8(buf);
            free(buf);
            text += line;
            if (line.length() > 0) {
                if (line.at(line.length() - 1) != '\\') {
                    QScriptSyntaxCheckResult check = QScriptEngine::checkSyntax(text);
                    if ( ( (check.state() == QScriptSyntaxCheckResult::Error) &&
                           (check.errorColumnNumber() == line.length()) ) ||
                         (check.state() == QScriptSyntaxCheckResult::Intermediate) )
                        text += '\\';
                    else
                        break;
                }
            }
            text[text.length() - 1] = '\n';
            prompt = "..> ";
        }
        if (!buf) break;
        add_history(text.toUtf8());
        QScriptValue result = engine.evaluate(text);
        if (engine.hasUncaughtException()) {
            result = engine.uncaughtException();
        }
        if (!result.isUndefined())
            puts(result.toString().toUtf8().constData());
    }
    return 0;
}
