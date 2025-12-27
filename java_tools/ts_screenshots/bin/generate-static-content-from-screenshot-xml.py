import xml.etree.ElementTree as ET
from pathlib import Path
import shutil
import urllib.parse

XML_FILE = "images/ScreenGeneratorTool.xml"
OUTPUT_HTML = "index.html"
IMAGES_DIR = "images"  # target folder for copied images
SOURCE_IMAGES_DIR = Path("images")  # source folder where your images really are

CSS_STYLE = """
body {
    font-family: Arial, sans-serif;
    background: #f7f9fc;
    color: #333;
    margin: 0;
    padding: 0;
    display: flex;
}
#toc {
    position: fixed;
    left: 0;
    top: 0;
    width: 280px;
    height: 100%;
    overflow-y: auto;
    background: #003366;
    color: white;
    padding: 20px;
    box-sizing: border-box;
}
#toc h2 { margin-top: 0; color: #fff; font-size: 1.2em; }
#toc input {
    width: 100%; padding: 6px; margin-bottom: 10px;
    border: none; border-radius: 4px; font-size: 0.95em;
}
#toc ul { list-style: none; padding-left: 10px; }
#toc li { margin: 5px 0; }
#toc a { text-decoration: none; color: #cce0ff; font-size: 0.95em; }
#toc a:hover { color: #fff; text-decoration: underline; }
#content { margin-left: 300px; padding: 20px; max-width: 1000px; }
h1 { color: #004080; }
h2 { color: #004080; border-bottom: 2px solid #004080; padding-top: 20px; }
h3 { margin-top: 15px; color: #0066cc; }
.field {
    background: white; border-radius: 8px; padding: 10px; margin: 8px 0;
    box-shadow: 0 2px 5px rgba(0,0,0,0.1);
}
.field img { max-width: 100%; display: block; margin-top: 10px; }
.tooltip { font-size: 0.9em; color: #555; margin-top: 4px; }
"""

JS_SCRIPT = """
<script>
function filterTOC() {
    let input = document.getElementById("searchBox").value.toLowerCase();
    let items = document.querySelectorAll("#toc li");
    items.forEach(li => {
        let text = li.textContent.toLowerCase();
        li.style.display = text.includes(input) ? "" : "none";
    });
}
</script>
"""

def copy_image(image_name: str):
    """Copy image from SOURCE_IMAGES_DIR to IMAGES_DIR if exists."""
    if not image_name:
        return None
    src = SOURCE_IMAGES_DIR / image_name
    if src.exists():
        Path(IMAGES_DIR).mkdir(exist_ok=True)
        dst = Path(IMAGES_DIR) / src.name
        if not dst.exists():
            shutil.copy(src, dst)
        return f"{IMAGES_DIR}/{src.name}"
    return None  # not found

def main():
    tree = ET.parse(XML_FILE)
    root = tree.getroot()

    Path(IMAGES_DIR).mkdir(exist_ok=True)

    toc_parts = [
        "<div id='toc'>",
        "<h2>Contents</h2>",
        "<input type='text' id='searchBox' onkeyup='filterTOC()' placeholder='Search...'>",
        "<ul>"
    ]

    content_parts = [
        "<div id='content'>",
        "<h1>rusEFI Configuration Guide</h1>"
    ]

    for keyword in root.findall(".//keyword"):
        menu_title = keyword.attrib.get("title", "Untitled Menu")
        menu_id = f"menu_{menu_title.replace(' ', '_')}"
        toc_parts.append(f"<li><a href='#{menu_id}'>{menu_title}</a><ul>")
        content_parts.append(f"<h2 id='{menu_id}'>{menu_title}</h2>")

        for dialog in keyword.findall(".//dialog"):
            dialog_title = dialog.attrib.get("dialogTitle", "Dialog")
            dialog_id = f"{menu_id}_{dialog_title.replace(' ', '_')}"
            toc_parts.append(f"<li><a href='#{dialog_id}'>{dialog_title}</a></li>")

            dialog_img = dialog.attrib.get("imageName", "")
            content_parts.append(f"<h3 id='{dialog_id}'>{dialog_title}</h3>")
            if dialog_img:
                img_path = copy_image(dialog_img)
                if img_path:
                    content_parts.append(f"<img src='{urllib.parse.quote_plus(img_path)}' alt='{dialog_title}'>")

            for field in dialog.findall(".//field"):
                ui_name = field.attrib.get("uiName", "Field")
                img = field.attrib.get("imageName", "")
                tooltip = field.attrib.get("tooltip", "")

                content_parts.append("<div class='field' id='{}'>")
                content_parts.append(f"<strong>{ui_name}</strong>")
                if tooltip:
                    tooltipHtml = "<br>".join(tooltip.split("\\n"))
                    content_parts.append(f"<div class='tooltip'>{tooltipHtml}</div>")
                if img:
                    img_path = copy_image(img)
                    if img_path:
                        content_parts.append(f"<img src='{urllib.parse.quote_plus(img_path)}' alt='{ui_name}'>")
                content_parts.append("</div>")

        toc_parts.append("</ul></li>")

    toc_parts.append("</ul></div>")
    content_parts.append("</div>")

    html_parts = [
        "<!DOCTYPE html>",
        "<html><head><meta charset='UTF-8'>",
        "<title>rusEFI Configuration Guide</title>",
        f"<style>{CSS_STYLE}</style>",
        JS_SCRIPT,
        "</head><body>",
        "\n".join(toc_parts),
        "\n".join(content_parts),
        "</body></html>"
    ]

    Path(OUTPUT_HTML).write_text("\n".join(html_parts), encoding="utf-8")
    print(f"Guide successfully created: {OUTPUT_HTML}")
    print(f"Images copied to folder: {IMAGES_DIR}/")

if __name__ == "__main__":
    main()
