Here is a summary of the help topic syntax and the rules for defining help content:

**1. Dialog/Panel Help (`topicHelp`)**
You can assign a help menu directly to any dialog, Curve Editor, or Table Editor using the `topicHelp` attribute.
*   **Web URL Syntax:** `topicHelp = "http://helpwebsite.com/helpdocs/help.html#anchor"`. This opens the specific page in a web browser.
*   **Local Cached PDF Syntax:** `topicHelp = "file://$getProjectsDirPath()/docs/HelpDoc_x.x.pdf#xxx"`. This opens a local PDF and supports deep links/anchors.
*   **Definition Rule:** `topicHelp` is defined directly within the component definition (e.g., right under the `dialog = ...` entry). Additionally, if you use cached PDFs, you **must** define the `helpManualDownloadRoot` URL in the `[TunerStudio]` section of the file so the application knows where to download the PDF if it is not found locally.

**2. Plain Text Help**
You can define plain text help that users can access via a menu.
*   **Syntax:**
    `help = helpReferenceName, "Help Title"`
    `webHelp = "http://..."` *(optional)*
    `text = "Your help text here. Use <br> for new lines."`.
*   **Definition Rule:** This is defined as an entity within the `[UserDefined]` or `[UiDialogs]` section, and it is triggered by referencing the `helpReferenceName` from a defined `[Menu]`.

**3. Setting Context Help (Tooltips)**
You can add short help tips (a small blue square with a question mark) to specific constants or variables in your dialogs.
*   **Syntax:** `constantName = "Your Help Text"`.
*   **Definition Rule:** These specific help tips **must** be defined inside the dedicated `[SettingContextHelp]` section.
