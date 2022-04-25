#include <new>
#include <stack>
#include <string>
#include <utility>
#include <stdio.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <propkey.h>
#include <propkey.h>
#include <propsys.h>

#include "libwpd/libwpd.h"
#include "callbacks.h"

// WPD requires all of these. Ugh, this is so ugly :(

void FilterCallbacks::endDocument() {}
void FilterCallbacks::startDocument() {}
void FilterCallbacks::definePageStyle(const WPXPropertyList &propList) {}
void FilterCallbacks::openPageSpan(const WPXPropertyList &propList) {}
void FilterCallbacks::closePageSpan() {}
void FilterCallbacks::openHeader(const WPXPropertyList &propList) {}
void FilterCallbacks::closeHeader() {}
void FilterCallbacks::openFooter(const WPXPropertyList &propList) {}
void FilterCallbacks::closeFooter() {}
void FilterCallbacks::defineParagraphStyle(const WPXPropertyList &propList,
                                           const WPXPropertyListVector &tabStops) {}
void FilterCallbacks::openParagraph(const WPXPropertyList &propList,
                                    const WPXPropertyListVector &tabStops) {}
void FilterCallbacks::closeParagraph() {}
void FilterCallbacks::defineCharacterStyle(const WPXPropertyList &propList) {}
void FilterCallbacks::openSpan(const WPXPropertyList &propList) {}
void FilterCallbacks::closeSpan() {}
void FilterCallbacks::defineSectionStyle(const WPXPropertyList &propList,
                                         const WPXPropertyListVector &columns) {}
void FilterCallbacks::openSection(const WPXPropertyList &propList,
                                  const WPXPropertyListVector &columns) {}
void FilterCallbacks::closeSection() {}
void FilterCallbacks::insertTab() {}
void FilterCallbacks::insertSpace() {}
void FilterCallbacks::insertLineBreak() {}
void FilterCallbacks::insertField(const WPXString &type,
                                  const WPXPropertyList &propList) {}
void FilterCallbacks::defineOrderedListLevel(const WPXPropertyList &propList) {}
void FilterCallbacks::defineUnorderedListLevel(const WPXPropertyList &propList) {}
void FilterCallbacks::openOrderedListLevel(const WPXPropertyList &propList) {}
void FilterCallbacks::openUnorderedListLevel(const WPXPropertyList &propList) {}
void FilterCallbacks::closeOrderedListLevel() {}
void FilterCallbacks::closeUnorderedListLevel() {}
void FilterCallbacks::openListElement(const WPXPropertyList &propList,
                                      const WPXPropertyListVector &tabStops) {}
void FilterCallbacks::closeListElement() {}
void FilterCallbacks::openFootnote(const WPXPropertyList &propList) {}
void FilterCallbacks::closeFootnote() {}
void FilterCallbacks::openEndnote(const WPXPropertyList &propList) {}
void FilterCallbacks::closeEndnote() {}
void FilterCallbacks::openComment(const WPXPropertyList &propList) {}
void FilterCallbacks::closeComment() {}
void FilterCallbacks::openTextBox(const WPXPropertyList &propList) {}
void FilterCallbacks::closeTextBox() {}
void FilterCallbacks::openTable(const WPXPropertyList &propList,
                                const WPXPropertyListVector &columns) {}
void FilterCallbacks::openTableRow(const WPXPropertyList &propList) {}
void FilterCallbacks::closeTableRow() {}
void FilterCallbacks::openTableCell(const WPXPropertyList &propList) {}
void FilterCallbacks::closeTableCell() {}
void FilterCallbacks::insertCoveredTableCell(const WPXPropertyList &propList) {}
void FilterCallbacks::closeTable() {}
void FilterCallbacks::openFrame(const WPXPropertyList &propList) {}
void FilterCallbacks::closeFrame() {}
void FilterCallbacks::insertBinaryObject(const WPXPropertyList &propList,
                                         const WPXBinaryData &data) {}
void FilterCallbacks::insertEquation(const WPXPropertyList &propList,
                                     const WPXString &data) {}

void FilterCallbacks::setDocumentMetaData(const WPXPropertyList &propList)
{
    WPXPropertyList::Iter propIter(propList);

    // I dunno how to handle these. There are dozens more, this is just a
    // start.
    for (propIter.rewind(); propIter.next(); ) {
        std::string key(propIter.key());
        std::string value(propIter()->getStr().cstr());
        std::wstring wvalue(value.begin(), value.end());

        if (key == "dc:subject") {
            this->props.push(make_pair(PKEY_FileDescription, wvalue));
        }
        if (key == "meta:initial-creator") {
            this->props.push(make_pair(PKEY_Author, wvalue));
        }
        if (key == "meta:keyword") {
            this->props.push(make_pair(PKEY_Keywords, wvalue));
        }
        if (key == "libwpd:comments") {
            this->props.push(make_pair(PKEY_Comment, wvalue));
        }
    }
    return;
}

void FilterCallbacks::insertText(const WPXString &text)
{
    std::string value(text.cstr());
    std::wstring wvalue(value.begin(), value.end());
    if (!this->searchtext.empty())
        this->searchtext.append(L"\n");
    this->searchtext.append(wvalue);
    return;
}
