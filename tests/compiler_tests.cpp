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
    testParserNestedContainers();
    testParserStyleOnLeafStatement();
    testParserStyleInsideButtonAndContainer();
    testParserLinkTarget();
    testParserStylesheetAndMeta();
    testParserOnHoverEvent();
    testParserFaviconAndRawHtml();

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

    std::cout << "All tests passed\n";
    return 0;
}
