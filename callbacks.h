#ifndef __CALLBACKS_H
#define __CALLBACKS_H

class FilterCallbacks: public WPXDocumentInterface
{
    public:
        virtual void setDocumentMetaData(const WPXPropertyList &propList);
        virtual void startDocument();
        virtual void endDocument();
        virtual void definePageStyle(const WPXPropertyList &propList);
        virtual void openPageSpan(const WPXPropertyList &propList);
        virtual void closePageSpan();
        virtual void openHeader(const WPXPropertyList &propList);
        virtual void closeHeader();
        virtual void openFooter(const WPXPropertyList &propList);
        virtual void closeFooter();
        virtual void defineParagraphStyle(const WPXPropertyList &propList,
                                          const WPXPropertyListVector &tabStops);
        virtual void openParagraph(const WPXPropertyList &propList,
                                   const WPXPropertyListVector &tabStops);
        virtual void closeParagraph();
        virtual void defineCharacterStyle(const WPXPropertyList &propList);
        virtual void openSpan(const WPXPropertyList &propList);
        virtual void closeSpan();
        virtual void defineSectionStyle(const WPXPropertyList &propList,
                                        const WPXPropertyListVector &columns);
        virtual void openSection(const WPXPropertyList &propList,
                                 const WPXPropertyListVector &columns);
        virtual void closeSection();
        virtual void insertTab();
        virtual void insertSpace();
        virtual void insertText(const WPXString &text);
        virtual void insertLineBreak();
        virtual void insertField(const WPXString &type,
                                 const WPXPropertyList &propList);
        virtual void defineOrderedListLevel(const WPXPropertyList &propList);
        virtual void defineUnorderedListLevel(const WPXPropertyList &propList);
        virtual void openOrderedListLevel(const WPXPropertyList &propList);
        virtual void openUnorderedListLevel(const WPXPropertyList &propList);
        virtual void closeOrderedListLevel();
        virtual void closeUnorderedListLevel();
        virtual void openListElement(const WPXPropertyList &propList,
                                     const WPXPropertyListVector &tabStops);
        virtual void closeListElement();
        virtual void openFootnote(const WPXPropertyList &propList);
        virtual void closeFootnote();
        virtual void openEndnote(const WPXPropertyList &propList);
        virtual void closeEndnote();
        virtual void openComment(const WPXPropertyList &propList);
        virtual void closeComment();
        virtual void openTextBox(const WPXPropertyList &propList);
        virtual void closeTextBox();
        virtual void openTable(const WPXPropertyList &propList,
                               const WPXPropertyListVector &columns);
        virtual void openTableRow(const WPXPropertyList &propList);
        virtual void closeTableRow();
        virtual void openTableCell(const WPXPropertyList &propList);
        virtual void closeTableCell();
        virtual void insertCoveredTableCell(const WPXPropertyList &propList);
        virtual void closeTable();
        virtual void openFrame(const WPXPropertyList &propList);
        virtual void closeFrame();
        virtual void insertBinaryObject(const WPXPropertyList &propList,
                                        const WPXBinaryData &data);
        virtual void insertEquation(const WPXPropertyList &propList,
                                    const WPXString &data);
        std::stack <std::pair<REFPROPERTYKEY,std::wstring>> props;
        std::wstring searchtext;
};

#endif
