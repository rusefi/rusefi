import xml.etree.ElementTree as ET
from pathlib import Path
import drawsvg
from PIL import Image
import base64
from io import BytesIO
import re

XML_FILE = "images/ScreenGeneratorTool.xml"
SOURCE_IMAGES_DIR = Path("images")  # source folder where your images really are

class Hyperlink(drawsvg.DrawingParentElement):
    TAG_NAME = 'a'
    def __init__(self, href, target=None, **kwargs):
        super().__init__(href=href, target=target, **kwargs)

def gen_svgs():
    tree = ET.parse(XML_FILE)
    root = tree.getroot()

    for keyword in root.findall(".//keyword"):
        menu_title = keyword.attrib.get("title", "Untitled Menu")

        for dialog in keyword.findall(".//dialog"):
            dialog_title = dialog.attrib.get("dialogTitle", "Dialog")
            dialog_img_name = dialog.attrib.get("imageName", "")

            dialog_img = Image.open(SOURCE_IMAGES_DIR / dialog_img_name)
            w, h = dialog_img.size
            svg = drawsvg.Drawing(w, h)

            img_bytes = BytesIO()
            dialog_img.save(img_bytes, "png")
            svg.append(drawsvg.Image(0, 0, w, h, data=img_bytes.getvalue(), embed=True, mime_type="image/png"))

            for field in dialog.findall(".//field"):
                ui_name = re.sub(r'(?<! )\([^()]+\)$', '', field.attrib.get("uiName", "Field"))
                img_name = field.attrib.get("imageName", "")
                x = field.attrib.get("x", 0)
                y = field.attrib.get("y", 0)
                img = Image.open(SOURCE_IMAGES_DIR / img_name)
                sw, sh = img.size

                tag = dialog_title.lower().replace(' ', '-') + "-" + ui_name.lower().replace(' ', '-')
                link = Hyperlink("#" + tag)
                link.append(drawsvg.Rectangle(x, y, sw, sh, fill='#00000000', stroke='blue', stroke_with='1px'))
                svg.append(link)

            svg.save_svg(SOURCE_IMAGES_DIR / dialog_img_name.replace("png", "svg"))


def main():
    gen_svgs()

if __name__ == "__main__":
    main()
