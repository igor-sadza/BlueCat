
Using VbTestClient with Biograph.
---------------------------------

Copy the following files into the External folder of your Biograph
installation:

	ConnectionClient.dll
	VbTestClient.exe
	VbTestClient.xml

For general instructions, refer to the document ConnectionInstrumentSDK.doc
located in the root of the Connection Instrument SDK.

Using the Connection Client API in your own VB projects.
-------------------------------------------------------

1) Copy the following files into your project folder:

	ConnectionClient.dll
	ConnectionClient.tlb

2) Add a reference to Connection Client Type Library:

	- on the "Project" menu, select "References...".
	- click the "Browse" button on the "References" dialog.
	- navigate to and select the copy of ConnectionClient.tlb that
	  is in your project folder.
	- click "Open" on the "Add Reference" dialog.

	VB adds "Thought Technology Ltd. Connection Client 1.0 Type Library"
	to the list of available references. Make certain it is checked.

3) Start using the Connection Client API calls in your code, using
   VbTestClient as a guideline.

You do not have to prefix the API symbols with "TtlCxc." if they do not
conflict with existing symbols. However, typing "TtlCxc." enables VB's
IntelliSense feature, prompting you with a list of the constants and
procedures available in the API.
