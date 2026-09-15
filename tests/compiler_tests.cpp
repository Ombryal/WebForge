#include <iostream>

#include "TestCases.h"

int main() {
    testLexerBasicPage();
    testLexerButtonProgram();
    testLexerSkipsComments();
    testLexerStringEscapeSequences();
    testLexerNumericLiteral();

    testParserBasicPage();
    testParserButtonProgram();
    testParserBasicElements();
    testParserListStatement();
    testParserContainerStatement();
    testParserNestedContainers();
    testParserStyleOnLeafStatement();
    testParserStyleInsideButtonAndContainer();
    testParserLinkTarget();
    testParserStylesheetAndMeta();
    testParserOnHoverEvent();
    testParserFaviconAndRawHtml();
    testParserHeadingLevel();
    testParserOrderedList();

    testCodegenBasicPage();
    testCodegenButtonProgram();
    testCodegenEscapesSpecialCharacters();
    testCodegenBasicElements();
    testCodegenListAndContainer();
    testCodegenInlineStyle();
    testCodegenLinkTargetAndRel();
    testCodegenStylesheetAndMeta();
    testCodegenIdAndClassAttributes();
    testCodegenNestedContainers();
    testCodegenClickAndHoverBothRender();
    testCodegenFavicon();
    testCodegenRawHtmlIsUnescaped();
    testCodegenHeadingLevels();
    testCodegenOrderedList();

    std::cout << "All tests passed\n";
    return 0;
}
