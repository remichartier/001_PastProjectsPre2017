Attribute VB_Name = "Module1"
Sub OneMailSaveAndDeleteAttachment()

 'Declaration
  Dim myItems, myItem, myAttachments, myAttachment As Object
    Dim myOrt As String
    Dim FileName As String
    Dim myOlApp As New Outlook.Application
    Dim myOlExp As Outlook.Explorer
    Dim myOlSel As Outlook.Selection
    Dim i As Integer
    
 'Ask for destination folder
    myOrt = "C:\Documents and Settings\remi chartier\My Documents\Remi\USA\36_email attachments\2009\"
    
    
    'work on selected items
     Set myOlExp = myOlApp.ActiveExplorer
    Set myOlSel = myOlExp.Selection
 
 'for all items do...
    For Each myItem In myOlSel
    
        'point on attachments
        Set myAttachments = myItem.Attachments
        
        'if there are some...
        If myAttachments.Count > 0 Then
        
            'add remark to message text
            'myItem.Body = myItem.Body & vbCrLf & _
            '    "Removed Attachments:" & vbCrLf
 'for all attachments do...
            For i = 1 To myAttachments.Count
            j = myAttachments.Count
                ' don't detach formatings
                If Right(myAttachments(i).FileName, 3) = "png" Then
                ElseIf Right(myAttachments(i).FileName, 3) = "jpg" Then
                ElseIf Right(myAttachments(i).FileName, 3) = "msg" Then
                    GoTo checkout
                
                Else
                          
                    'save them to destination
                    'add date to Filename
                    FileName = Format(myItem.CreationTime, "yyyymmdd_hhnnss_") & myAttachments(i).DisplayName

                
                    myAttachments(i).SaveAsFile myOrt & _
                        FileName

                    'add name and destination to message text
                    'myItem.Body = myItem.Body & _
                     '   "File: " & myOrt & _
                      '  FileName & vbCrLf
             End If
              
              
              Next i
            
           'for all attachments do...
                While myAttachments.Count > 0
                    'If Right(myAttachments(i).FileName, 3) = "png" Then
                    'ElseIf Right(myAttachments(i).FileName, 3) = "jpg" Then
                    'Else
                
                        'remove it (use this method in Outlook XP)
                        'myAttachments(i).Remove 1
                
                        'remove it (use this method in Outlook 2000)
                        myAttachments(1).Delete
                    'End If
                Wend

            
            
            
            'save item without attachments
            myItem.Save
        End If
checkout:
        
    Next
    
    'free variables
    Set myItems = Nothing
    Set myItem = Nothing
    Set myAttachments = Nothing
    Set myAttachment = Nothing
    Set myOlSel = Nothing
    Set myOlApp = Nothing
    Set myOlExp = Nothing
 
  
    
End Sub

Sub FolderDetachments()

    Dim myOlApp As Outlook.Folder

    Dim Items, Item As Object
    Dim FileName, FileName1 As String
    Dim Atmt As Attachment
    Dim i As Integer
    
   'work on selected items
    ' Set myOlApp = Outlook.Folder
    
    For Each Item In myOlApp
        For Each Atmt In Item.Attachments
            'add date to Filename
                    FileName1 = Format(Item.CreationTime, "yyyymmdd_hhnnss_") & Atmt.DisplayName
       
             
            FileName = "C:\Documents and Settings\remi chartier\My Documents\Remi\USA\36_email attachments\2009\" & FileName1
            Atmt.SaveAsFile FileName1
            i = i + 1
        Next Atmt
        'for all attachments do...
        For Each Atmt In Item.Attachments
            While Item.Attachments.Count > 0
                
                        'remove it (use this method in Outlook XP)
                        'myAttachments(i).Remove 1
                
                        'remove it (use this method in Outlook 2000)
                        Item.Attachments(1).Delete
                    
            Wend
        Next Atmt
        
        'save item without attachments
            Item.Save
       
    Next Item
    
 
 
   'free variables
    Set Items = Nothing
    Set Item = Nothing
    Set myAttachments = Nothing
    Set myAttachment = Nothing
    Set myOlSel = Nothing
    Set myOlApp = Nothing
    Set myOlExp = Nothing
    Set Atmt = Nothing

End Sub
Sub EraseAttachments()
'Declaration
  Dim myItems, myItem, myAttachments, myAttachment As Object
    Dim myOrt As String
    Dim FileName As String
    Dim myOlApp As New Outlook.Application
    Dim myOlExp As Outlook.Explorer
    Dim myOlSel As Outlook.Selection
    Dim i As Integer
    
 'Ask for destination folder
    myOrt = "C:\Documents and Settings\remi chartier\My Documents\Remi\USA\36_email attachments\2009\"
    
    
    'work on selected items
     Set myOlExp = myOlApp.ActiveExplorer
    Set myOlSel = myOlExp.Selection
 
 'for all items do...
    For Each myItem In myOlSel
    
        'point on attachments
        Set myAttachments = myItem.Attachments
        
        'if there are some...
        If myAttachments.Count > 0 Then
        
            'add remark to message text
            'myItem.Body = myItem.Body & vbCrLf & _
            '    "Removed Attachments:" & vbCrLf
           
           
           'for all attachments do...
                While myAttachments.Count > 0
                    'If Right(myAttachments(i).FileName, 3) = "png" Then
                    'ElseIf Right(myAttachments(i).FileName, 3) = "jpg" Then
                    'Else
                
                        'remove it (use this method in Outlook XP)
                        'myAttachments(i).Remove 1
                
                        'remove it (use this method in Outlook 2000)
                        myAttachments(1).Delete
                    'End If
                Wend

            
            
            
            'save item without attachments
            myItem.Save
        End If
checkout:
        
    Next
    
    'free variables
    Set myItems = Nothing
    Set myItem = Nothing
    Set myAttachments = Nothing
    Set myAttachment = Nothing
    Set myOlSel = Nothing
    Set myOlApp = Nothing
    Set myOlExp = Nothing
 
  
 
End Sub
