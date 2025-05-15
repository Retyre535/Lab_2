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
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {

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
    resultList->InsertColumn(0, "Operation", wxLIST_FORMAT_LEFT, 200);
    resultList->InsertColumn(1, "Result", wxLIST_FORMAT_LEFT, 500);
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

            arr.Append(10);
            AddIntResult("After Append(10) size", arr.GetSize());
            AddIntResult("Last element", arr.Get(arr.GetSize() - 1));

            arr.Prepend(0);
            AddIntResult("After Prepend(0) size", arr.GetSize());
            AddIntResult("First element", arr.Get(0));

            arr.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", arr.GetSize());
            AddIntResult("Element at index 2", arr.Get(2));

            arr.Resize(3);
            AddIntResult("After Resize(3) size", arr.GetSize());

        }
        else if (sequenceType == "LinkedList") {
            LinkedList<int> list(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", list.GetSize());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());
            AddIntResult("Last element", list.GetLast());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());
            AddIntResult("First element", list.GetFirst());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
            AddIntResult("Element at index 2", list.Get(2));

        }
        else if (sequenceType == "ArraySequence") {
            ArraySequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());

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

        }
        else if (sequenceType == "ListSequence") {
            ListSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());

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

        }
        else if (sequenceType == "AdaptiveSequence") {
            AdaptiveSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

        }
        else if (sequenceType == "SegmentedList") {
            SegmentedList<int> list;
            for (size_t i = 0; i < inputItems.GetCount(); i++) {
                list.Append(items[i]);
            }

            AddIntResult("Initial size", list.GetSize());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
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

wxIMPLEMENT_APP(SequenceTesterApp);