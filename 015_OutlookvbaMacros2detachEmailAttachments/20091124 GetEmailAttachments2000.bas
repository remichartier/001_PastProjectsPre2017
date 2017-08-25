Attribute VB_Name = "GetEmailAttachments"
Option Explicit

'************************** Õ¿Õ- **************************
'*** Code by Martin Green ******** martin@fontstuff.com ***
'******* Office Tips Web Site - www.fontstuff.com *********
'**********************************************************

Sub GetAttachments()
' This Outlook macro checks a the Outlook Inbox for messages
' with attached files (of any type) and saves them to disk.
' NOTE: make sure the specified save folder exists before
' running the macro.
    On Error GoTo GetAttachments_err
' Declare variables
    Dim ns As NameSpace
    Dim Inbox As MAPIFolder
    Dim Item As Object
    Dim Atmt As Attachment
    Dim FileName As String
    Dim i As Integer
    Set ns = GetNamespace("MAPI")
    Set Inbox = ns.GetDefaultFolder(olFolderInbox)
    i = 0
' Check Inbox for messages and exit of none found
    If Inbox.Items.Count = 0 Then
        MsgBox "There are no messages in the Inbox.", vbInformation, _
               "Nothing Found"
        Exit Sub
    End If
' Check each message for attachments
    For Each Item In Inbox.Items
' Save any attachments found
        For Each Atmt In Item.Attachments
        ' This path must exist! Change folder name as necessary.
            FileName = "C:\Email Attachments\" & Atmt.FileName
            Atmt.SaveAsFile FileName
            i = i + 1
         Next Atmt
    Next Item
' Show summary message
    If i > 0 Then
        MsgBox "I found " & i & " attached files." _
        & vbCrLf & "I have saved them into the C:\Email Attachments folder." _
        & vbCrLf & vbCrLf & "Have a nice day.", vbInformation, "Finished!"
    Else
        MsgBox "I didn't find any attached files in your mail.", vbInformation, "Finished!"
    End If
' Clear memory
GetAttachments_exit:
    Set Atmt = Nothing
    Set Item = Nothing
    Set ns = Nothing
    Exit Sub
' Handle errors
GetAttachments_err:
    MsgBox "An unexpected error has occurred." _
        & vbCrLf & "Please note and report the following information." _
        & vbCrLf & "Macro Name: GetAttachments" _
        & vbCrLf & "Error Number: " & Err.Number _
        & vbCrLf & "Error Description: " & Err.Description _
        , vbCritical, "Error!"
    Resume GetAttachments_exit
End Sub

Sub SaveAttachmentsToFolder()
' This Outlook macro checks a named subfolder in the Outlook Inbox
' (here the "Sales Reports" folder) for messages with attached
' files of a specific type (here file with an "xls" extension)
' and saves them to disk. Saved files are timestamped. The user
' can choose to view the saved files in Windows Explorer.
' NOTE: make sure the specified subfolder and save folder exist
' before running the macro.
    On Error GoTo SaveAttachmentsToFolder_err
' Declare variables
    Dim ns As NameSpace
    Dim Inbox As MAPIFolder
    Dim SubFolder As MAPIFolder
    Dim Item As Object
    Dim Atmt As Attachment
    Dim FileName As String
    Dim i As Integer
    Dim varResponse As VbMsgBoxResult
    Set ns = GetNamespace("MAPI")
    Set Inbox = ns.GetDefaultFolder(olFolderInbox)
    Set SubFolder = Inbox.Folders("Sales Reports") ' Enter correct subfolder name.
    i = 0
' Check subfolder for messages and exit of none found
    If SubFolder.Items.Count = 0 Then
        MsgBox "There are no messages in the Sales Reports folder.", vbInformation, _
               "Nothing Found"
        Exit Sub
    End If
' Check each message for attachments
    For Each Item In SubFolder.Items
        For Each Atmt In Item.Attachments
' Check filename of each attachment and save if it has "xls" extension
            If Right(Atmt.FileName, 3) = "xls" Then
            ' This path must exist! Change folder name as necessary.
                FileName = "C:\Email Attachments\" & _
                    Format(Item.CreationTime, "yyyymmdd_hhnnss_") & Atmt.FileName
                Atmt.SaveAsFile FileName
                i = i + 1
            End If
        Next Atmt
    Next Item
' Show summary message
    If i > 0 Then
        varResponse = MsgBox("I found " & i & " attached files." _
        & vbCrLf & "I have saved them into the C:\Email Attachments folder." _
        & vbCrLf & vbCrLf & "Would you like to view the files now?" _
        , vbQuestion + vbYesNo, "Finished!")
' Open Windows Explorer to display saved files if user chooses
        If varResponse = vbYes Then
            Shell "Explorer.exe /e,C:\Email Attachments", vbNormalFocus
        End If
    Else
        MsgBox "I didn't find any attached files in your mail.", vbInformation, "Finished!"
    End If
' Clear memory
SaveAttachmentsToFolder_exit:
    Set Atmt = Nothing
    Set Item = Nothing
    Set ns = Nothing
    Exit Sub
' Handle Errors
SaveAttachmentsToFolder_err:
    MsgBox "An unexpected error has occurred." _
        & vbCrLf & "Please note and report the following information." _
        & vbCrLf & "Macro Name: GetAttachments" _
        & vbCrLf & "Error Number: " & Err.Number _
        & vbCrLf & "Error Description: " & Err.Description _
        , vbCritical, "Error!"
    Resume SaveAttachmentsToFolder_exit
End Sub
