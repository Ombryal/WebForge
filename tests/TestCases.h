#pragma once

// Declarations for every test case, grouped by which compiler stage they
// exercise. Definitions live in lexer_tests.cpp, parser_tests.cpp, and
// codegen_tests.cpp; compiler_tests.cpp just includes this and calls them.

// --- Lexer ---
void testLexerBasicPage();
void testLexerButtonProgram();
void testLexerSkipsComments();

// --- Parser ---
void testParserBasicPage();
void testParserButtonProgram();
void testParserBasicElements();
void testParserListStatement();
void testParserContainerStatement();
void testParserRejectsNestedContainer();
void testParserStyleOnLeafStatement();
void testParserStyleInsideButtonAndContainer();
void testParserLinkTarget();

// --- Codegen ---
void testCodegenBasicPage();
void testCodegenButtonProgram();
void testCodegenEscapesSpecialCharacters();
void testCodegenBasicElements();
void testCodegenListAndContainer();
void testCodegenInlineStyle();
void testCodegenLinkTargetAndRel();
