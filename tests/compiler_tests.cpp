#include <iostream>

#include "TestCases.h"

int main() {
    testLexerBasicPage();
    testLexerButtonProgram();
    testLexerSkipsComments();

    testParserBasicPage();
    testParserButtonProgram();
    testParserBasicElements();
    testParserListStatement();
    testParserContainerStatement();
    testParserRejectsNestedContainer();
    testParserStyleOnLeafStatement();
    testParserStyleInsideButtonAndContainer();
    testParserLinkTarget();
    testParserStylesheetAndMeta();

    testCodegenBasicPage();
    testCodegenButtonProgram();
    testCodegenEscapesSpecialCharacters();
    testCodegenBasicElements();
    testCodegenListAndContainer();
    testCodegenInlineStyle();
    testCodegenLinkTargetAndRel();
    testCodegenStylesheetAndMeta();
    testCodegenIdAndClassAttributes();

    std::cout << "All tests passed\n";
    return 0;
}
