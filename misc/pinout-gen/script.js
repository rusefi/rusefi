var connectorData = [
///DATA///
];

function addRow(table, pin, pdiv) {
  var template = document.getElementById("table-template");
  var clone = template.content.cloneNode(true);
  var row = clone.querySelector(".data");
  var pdata = clone.querySelector(".pin-data");
  var idata = clone.querySelector(".ts-data");
  var tdata = clone.querySelector(".type-data");
  var fdata = clone.querySelector(".function-data");
  var cdata = clone.querySelector(".color-data");
  pdata.textContent = pin.pin;
  pdata.dataset.type = pin.type;
  idata.textContent = pin.ts_name;
  tdata.textContent = pin.type
  fdata.textContent = pin.function;
  cdata.textContent = pin.color
  if (pdiv) {
    row.addEventListener('click', function(table, pin, pdiv) {
      clickPin(table.parentElement.parentElement.parentElement.querySelector(".info-table tbody"), pin, pdiv);
      table.parentElement.parentElement.parentElement.scrollIntoView()
    }.bind(null, table, pin, pdiv));
  }
  table.appendChild(clone);
}

function clickPin(table, pin, pdiv) {
  table.parentElement.style.display = "table";
  table.innerHTML = "";
  addRow(table, pin, pdiv);
  var pins = document.querySelectorAll(".pin-marker");
  for (var i = 0; i < pins.length; i++) {
    if (pins[i].dataset.type == pin.type) {
      pins[i].classList.add("highlight");
    } else {
      pins[i].classList.remove("highlight");
    }
    pins[i].classList.remove("selected");
  }
  pdiv.classList.add("selected");
}

window.addEventListener('load', function() {
  for (var c = 0; c < connectorData.length; c++) {
    var connector = JSON.parse(connectorData[c]);
    var template = document.getElementById("connector-template");
    var clone = template.content.cloneNode(true);
    document.body.appendChild(clone);
    var sdiv = document.body.lastChild.previousSibling;
    var img = sdiv.querySelector(".connector-img");
    img.addEventListener('load', function(connector, sdiv, img) {
      var cdiv = sdiv.querySelector(".connector-div");
      var ptemplate = document.getElementById("pin-template");
      var imgHeight = img.naturalHeight;
      var imgWidth = img.naturalWidth;
      var table = sdiv.querySelector(".info-table").querySelector("tbody");
      var fullTable = sdiv.querySelector(".pinout-table").querySelector("tbody");
      for (var i = 0; i < connector.pins.length; i++) {
        var pin = connector.pins[i];
        if (!pin.pin) {
          continue;
        }
        var pinfo = {};
        for (var ii = 0; ii < connector.info.pins.length; ii++) {
          if (connector.info.pins[ii].pin == pin.pin) {
            pinfo = connector.info.pins[ii];
            break;
          }
        }
        if (!pinfo.x) {
          addRow(fullTable, connector.pins[i], null);
          continue;
        }
        var closest = 1000000;
        for (var ii = 0; ii < connector.info.pins.length; ii++) {
          var tinfo = connector.info.pins[ii];
          var distance = Math.pow((tinfo.x - pinfo.x), 2) + Math.pow((tinfo.y - pinfo.y), 2);
          if (tinfo.pin != pin.pin && (!closest || distance < closest)) {
            closest = distance;
          }
        }
        var pclone = ptemplate.content.cloneNode(true);
        var pdiv = pclone.querySelector("div");
        pdiv.textContent = pinfo.pin;
        pdiv.style.top = ((pinfo.y / imgHeight) * 100) + "%";
        pdiv.style.left = ((pinfo.x / imgWidth) * 100) + "%";
        pdiv.dataset.type = pin.type;
        pdiv.addEventListener("click", function(table, pin, pdiv) {
          clickPin(table, pin, pdiv);
        }.bind(null, table, pin, pdiv));
        closest = Math.sqrt(closest);
        var divheight = cdiv.clientHeight;
        var divwidth = cdiv.clientWidth;
        var mult = cdiv.querySelector("img").naturalHeight / divheight;
        var newheight = (closest / mult)
        var pxheight = divheight * 0.08;
        if (newheight < pxheight) {
          pxheight = newheight - 6;
        }
        var height = (pxheight / divheight) * 100;
        var width = (pxheight / divwidth) * 100;
        pdiv.style.height = height + "%";
        pdiv.style.width = width + "%";
        pdiv.style.marginTop = "-" + (width / 2) + "%";
        pdiv.style.marginLeft = "-" + (width / 2) + "%";
        pdiv.style.fontSize = (height * 1.8) + "px";
        cdiv.appendChild(pdiv);
        addRow(fullTable, connector.pins[i], pdiv);
      }
      pempty = true;
      iempty = true;
      tempty = true;
      fempty = true;
      cempty = true;
      rows = fullTable.children;
      for (var i = 0; i < rows.length; i++) {
          pempty = rows[i].children[0].textContent.length > 0 ? false : pempty;
          iempty = rows[i].children[1].textContent.length > 0 ? false : iempty;
          tempty = rows[i].children[2].textContent.length > 0 ? false : tempty;
          fempty = rows[i].children[3].textContent.length > 0 ? false : fempty;
          cempty = rows[i].children[4].textContent.length > 0 ? false : cempty;
      }
      fullTableHead = sdiv.querySelector(".pinout-table>thead")
      if (pempty) {
        fullTableHead.querySelector('thead>tr>th.pin-header').style.display = 'none';
        for (var i = 0; i < rows.length; i++) {
          rows[i].children[0].style.display = 'none';
        }
      }
      if (iempty) {
        fullTableHead.querySelector('thead>tr>th.ts-header').style.display = 'none';
        for (var i = 0; i < rows.length; i++) {
          rows[i].children[1].style.display = 'none';
        }
      }
      if (tempty) {
        fullTableHead.querySelector('thead>tr>th.type-header').style.display = 'none';
        for (var i = 0; i < rows.length; i++) {
          rows[i].children[2].style.display = 'none';
        }
      }
      if (fempty) {
        fullTableHead.querySelector('thead>tr>th.function-header').style.display = 'none';
        for (var i = 0; i < rows.length; i++) {
          rows[i].children[3].style.display = 'none';
        }
      }
      if (cempty) {
        fullTableHead.querySelector('thead>tr>th.color-header').style.display = 'none';
        for (var i = 0; i < rows.length; i++) {
          rows[i].children[4].style.display = 'none';
        }
      }
    }.bind(null, connector, sdiv, img));
    img.src = connector.info.image.file;
  }
});
