#include <wx/wx.h>
#include <wx/listctrl.h>
#include "Sequences.h"

class SequenceTesterApp : public wxApp {
public:
    virtual bool OnInit();
};

class SequenceTesterFrame : public wxFrame {
public:
    SequenceTesterFrame(const wxString& title);

private:
    wxChoice* sequenceTypeChoice;
    wxTextCtrl* inputText;
    wxButton* testButton;
    wxListCtrl* resultList;

    void OnTestButtonClick(wxCommandEvent& event);
    void TestSequence(wxString sequenceType, wxString input);

    void AddResult(wxString operation, wxString result);
    void AddIntResult(wxString operation, int result);
    void AddBoolResult(wxString operation, bool result);

    DECLARE_EVENT_TABLE()
};

enum {
    ID_TestButton = 1
};

BEGIN_EVENT_TABLE(SequenceTesterFrame, wxFrame)
EVT_BUTTON(ID_TestButton, SequenceTesterFrame::OnTestButtonClick)
END_EVENT_TABLE()

bool SequenceTesterApp::OnInit() {
    SequenceTesterFrame* frame = new SequenceTesterFrame("Sequence Tester");
    frame->Show(true);
    return true;
}

SequenceTesterFrame::SequenceTesterFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700)) {

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Sequence type selection
    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Sequence Type:");
    sequenceTypeChoice = new wxChoice(panel, wxID_ANY);
    sequenceTypeChoice->Append("DynamicArray");
    sequenceTypeChoice->Append("LinkedList");
    sequenceTypeChoice->Append("ArraySequence");
    sequenceTypeChoice->Append("ListSequence");
    sequenceTypeChoice->Append("AdaptiveSequence");
    sequenceTypeChoice->Append("SegmentedList");
    sequenceTypeChoice->Append("MutableArraySequence");
    sequenceTypeChoice->Append("ImmutableArraySequence");
    sequenceTypeChoice->Append("MutableListSequence");
    sequenceTypeChoice->Append("ImmutableListSequence");
    sequenceTypeChoice->SetSelection(0);

    wxBoxSizer* typeSizer = new wxBoxSizer(wxHORIZONTAL);
    typeSizer->Add(typeLabel, 0, wxRIGHT, 10);
    typeSizer->Add(sequenceTypeChoice, 1);
    vbox->Add(typeSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Input
    wxStaticText* inputLabel = new wxStaticText(panel, wxID_ANY, "Input (comma separated integers):");
    inputText = new wxTextCtrl(panel, wxID_ANY, "1,2,3,4,5", wxDefaultPosition, wxDefaultSize);
    vbox->Add(inputLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(inputText, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    // Test button
    testButton = new wxButton(panel, ID_TestButton, "Run Tests");
    vbox->Add(testButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Results
    resultList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    resultList->InsertColumn(0, "Operation", wxLIST_FORMAT_LEFT, 300);
    resultList->InsertColumn(1, "Result", wxLIST_FORMAT_LEFT, 600);
    vbox->Add(resultList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vbox);
    Center();
}

void SequenceTesterFrame::OnTestButtonClick(wxCommandEvent& event) {
    resultList->DeleteAllItems();

    wxString sequenceType = sequenceTypeChoice->GetStringSelection();
    wxString input = inputText->GetValue();

    TestSequence(sequenceType, input);
}

void SequenceTesterFrame::TestSequence(wxString sequenceType, wxString input) {
    // Parse input
    wxArrayString inputItems = wxSplit(input, ',');
    if (inputItems.IsEmpty()) {
        wxMessageBox("Please enter some numbers separated by commas", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int* items = new int[inputItems.GetCount()];
    for (size_t i = 0; i < inputItems.GetCount(); i++) {
        long value;
        if (!inputItems[i].ToLong(&value)) {
            wxMessageBox("Invalid number: " + inputItems[i], "Error", wxOK | wxICON_ERROR);
            delete[] items;
            return;
        }
        items[i] = (int)value;
    }

    try {
        if (sequenceType == "DynamicArray") {
            DynamicArray<int> arr(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", arr.GetSize());
            AddIntResult("Element at index 0", arr.Get(0));
            AddIntResult("Element at index 1", arr.Get(1));

            arr.Append(10);
            AddIntResult("After Append(10) size", arr.GetSize());
            AddIntResult("Last element", arr.Get(arr.GetSize() - 1));

            arr.Prepend(0);
            AddIntResult("After Prepend(0) size", arr.GetSize());
            AddIntResult("First element", arr.Get(0));

            arr.Insert(-99, 2);
            AddIntResult("After Insert(-99, 2) size", arr.GetSize());
            AddIntResult("Element at index 2", arr.Get(2));

            arr.Resize(3);
            AddIntResult("After Resize(3) size", arr.GetSize());

            arr.Set(1, 42);
            AddIntResult("After Set(1, 42)", arr.Get(1));

            DynamicArray<int> arrCopy(arr);
            AddIntResult("Copy constructor size", arrCopy.GetSize());
            AddIntResult("Copy element at 0", arrCopy.Get(0));

            arr[0] = 100;
            AddIntResult("After operator[] set", arr[0]);

            // Test exception
            try {
                arr.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "LinkedList") {
            LinkedList<int> list(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", list.GetSize());
            AddIntResult("First element", list.GetFirst());
            AddIntResult("Last element", list.GetLast());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());
            AddIntResult("Last element", list.GetLast());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());
            AddIntResult("First element", list.GetFirst());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
            AddIntResult("Element at index 2", list.Get(2));

            LinkedList<int>* subList = list.GetSubList(1, 3);
            AddIntResult("SubList(1,3) size", subList->GetSize());
            delete subList;

            LinkedList<int> listCopy(list);
            AddIntResult("Copy constructor size", listCopy.GetSize());
            AddIntResult("Copy first element", listCopy.GetFirst());

            list[0] = 100;
            AddIntResult("After operator[] set", list[0]);

            LinkedList<int>* concatList = list.Concat(&listCopy);
            AddIntResult("Concat size", concatList->GetSize());
            delete concatList;

            // Test exception
            try {
                list.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "ArraySequence") {
            ArraySequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ArraySequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            // Test Map
            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            // Test TryGet
            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            // Test TryFind
            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            // Test From
            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            // Test Zip
            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            // Test exception
            try {
                seq.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "ListSequence") {
            ListSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ListSequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            // Test Map
            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            // Test TryGet
            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            // Test TryFind
            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            // Test From
            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            // Test Zip
            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            // Test exception
            try {
                seq.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "AdaptiveSequence") {
            AdaptiveSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            // Force switch to list by adding many elements
            if (seq.GetSize() < AdaptiveSequenceSwitch) {
                for (int i = 0; i < AdaptiveSequenceSwitch; i++) {
                    seq.Append(i);
                }
                AddIntResult("After force switch size", seq.GetSize());
                AddResult("Switch check", "Should have switched to list");
            }

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            AdaptiveSequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            // Test Map
            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            // Test TryGet
            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            // Test TryFind
            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            // Test From
            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            // Test Zip
            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            // Test exception
            try {
                seq.Get(100000);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "SegmentedList") {
            SegmentedList<int> list;
            for (size_t i = 0; i < inputItems.GetCount(); i++) {
                list.Append(items[i]);
            }

            AddIntResult("Initial size", list.GetSize());
            AddIntResult("First element", list.GetFirst());
            AddIntResult("Last element", list.GetLast());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());
            AddIntResult("Last element", list.GetLast());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());
            AddIntResult("First element", list.GetFirst());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
            AddIntResult("Element at index 2", list.Get(2));

            // Test segment splitting
            for (int i = 0; i < 50; i++) {
                list.Append(i);
            }
            AddIntResult("After multiple appends size", list.GetSize());

            Sequence<int>* subSeq = list.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            SegmentedList<int> listCopy(list);
            AddIntResult("Copy constructor size", listCopy.GetSize());
            AddIntResult("Copy first element", listCopy.GetFirst());

            list[0] = 100;
            AddIntResult("After operator[] set", list[0]);

            Sequence<int>* concatSeq = list.Concat(&listCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            // Test Map
            Sequence<int>* mappedSeq = list.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            // Test TryGet
            int value;
            bool found = list.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            // Test TryFind
            found = list.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            // Test From
            Sequence<int>* fromSeq = list.From(list);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            // Test Zip
            Sequence<int>* zippedSeq = list.Zip(list);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            // Test exception
            try {
                list.Get(100000);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "MutableArraySequence") {
            MutableArraySequence<int>* seq = new MutableArraySequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            // ��������� �������������� ��������
            seq->Append(10);
            AddIntResult("After Append(10) size", seq->GetSize());
            AddIntResult("Last element", seq->GetLast());

            seq->Prepend(0);
            AddIntResult("After Prepend(0) size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());

            seq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq->GetSize());
            AddIntResult("Element at index 2", seq->Get(2));

            // ��������� ����������� �����������
            MutableArraySequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            // ��������� Instance() - ������ ���������� this
            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is same object", (instance == seq));

            // ��������� Clone()
            Sequence<int>* cloned = seq->Clone();
            AddIntResult("Clone size", cloned->GetSize());
            AddIntResult("Clone first element", cloned->GetFirst());
            delete cloned;

            // ��������� �����...
            delete seq;
        }
        else if (sequenceType == "ImmutableArraySequence") {
            ImmutableArraySequence<int>* seq = new ImmutableArraySequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            // ��� immutable ������������������ �������� ������ ���������� ����� ����������
            Sequence<int>* appendedSeq = seq->Clone();
            appendedSeq->Append(10);
            AddIntResult("After Append(10) size", appendedSeq->GetSize());
            AddIntResult("Last element after append", appendedSeq->GetLast());
            delete appendedSeq;

            Sequence<int>* prependedSeq = seq->Clone();
            prependedSeq->Prepend(0);
            AddIntResult("After Prepend(0) size", prependedSeq->GetSize());
            AddIntResult("First element after prepend", prependedSeq->GetFirst());
            delete prependedSeq;

            Sequence<int>* insertedSeq = seq->Clone();
            insertedSeq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", insertedSeq->GetSize());
            AddIntResult("Element at index 2 after insert", insertedSeq->Get(2));
            delete insertedSeq;

            Sequence<int>* subSeq = seq->GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            // �������� ������������ �����������
            ImmutableArraySequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            // �������� Instance() - ������ ���������� ����
            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is different object", (instance != seq));
            delete instance;

            // ��������� �����...
            delete seq;
        }
        else if (sequenceType == "MutableListSequence") {
            MutableListSequence<int>* seq = new MutableListSequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            // ��������� �������������� ��������
            seq->Append(10);
            AddIntResult("After Append(10) size", seq->GetSize());
            AddIntResult("Last element", seq->GetLast());

            seq->Prepend(0);
            AddIntResult("After Prepend(0) size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());

            seq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq->GetSize());
            AddIntResult("Element at index 2", seq->Get(2));

            // ��������� ����������� �����������
            MutableListSequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            // ��������� Instance() - ������ ���������� this
            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is same object", (instance == seq));

            // ��������� Clone()
            Sequence<int>* cloned = seq->Clone();
            AddIntResult("Clone size", cloned->GetSize());
            AddIntResult("Clone first element", cloned->GetFirst());
            delete cloned;

            // ��������� �����...
            delete seq;
        }
        else if (sequenceType == "ImmutableListSequence") {
            ImmutableListSequence<int>* seq = new ImmutableListSequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            // ��� immutable ������������������ �������� ������ ���������� ����� ����������
            Sequence<int>* appendedSeq = seq->Clone();
            appendedSeq->Append(10);
            AddIntResult("After Append(10) size", appendedSeq->GetSize());
            AddIntResult("Last element after append", appendedSeq->GetLast());
            delete appendedSeq;

            Sequence<int>* prependedSeq = seq->Clone();
            prependedSeq->Prepend(0);
            AddIntResult("After Prepend(0) size", prependedSeq->GetSize());
            AddIntResult("First element after prepend", prependedSeq->GetFirst());
            delete prependedSeq;

            Sequence<int>* insertedSeq = seq->Clone();
            insertedSeq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", insertedSeq->GetSize());
            AddIntResult("Element at index 2 after insert", insertedSeq->Get(2));
            delete insertedSeq;

            Sequence<int>* subSeq = seq->GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            // �������� ������������ �����������
            ImmutableListSequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            // �������� Instance() - ������ ���������� ����
            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is different object", (instance != seq));
            delete instance;

            // ��������� �����...
            delete seq;
        }
    }
    catch (const IndexOutOfRange& e) {
        AddResult("Error", e.what());
    }
    catch (const std::exception& e) {
        AddResult("Error", e.what());
    }

    delete[] items;
}

void SequenceTesterFrame::AddResult(wxString operation, wxString result) {
    long index = resultList->InsertItem(resultList->GetItemCount(), operation);
    resultList->SetItem(index, 1, result);
}

void SequenceTesterFrame::AddIntResult(wxString operation, int result) {
    AddResult(operation, wxString::Format("%d", result));
}

void SequenceTesterFrame::AddBoolResult(wxString operation, bool result) {
    AddResult(operation, result ? "true" : "false");
}

wxIMPLEMENT_APP(SequenceTesterApp);