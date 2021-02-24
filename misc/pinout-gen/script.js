var connectorYaml = [
///DATA///
];

function addRow(table, pin, pdiv) {
  var template = document.getElementById("table-template");
  var clone = template.content.cloneNode(true);
  var row = clone.querySelector(".data");
  var pdata = clone.querySelector(".pin-data");
  var idata = clone.querySelector(".id-data");
  var tdata = clone.querySelector(".type-data");
  var fdata = clone.querySelector(".function-data");
  var cdata = clone.querySelector(".color-data");
  pdata.textContent = pin.pin;
  idata.textContent = pin.id;
  tdata.textContent = pin.type
  fdata.textContent = pin.function;
  cdata.textContent = pin.color
  row.addEventListener('click', function(pin, pdiv) {
    clickPin(pin, pdiv);
  }.bind(null, pin, pdiv));
  table.appendChild(clone);
}

function clickPin(table, pin, pdiv) {
  table.innerHTML = "";
  if (Array.isArray(pin.id)) {
    var pinIds = pin.id.filter((value, index) => {
      return pin.id.indexOf(value) === index;
    });
    for (var i = 0; i < pinIds.length; i++) {
      addRow(table, {pin: pin.pin, id: pinIds[i], function: pin.function, type: pin.type}, pdiv);
    }
  } else {
    addRow(table, pin, pdiv);
  }
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

function adjustMarkers(cdiv) {
  var cdiv = document.querySelectorAll(".connector-div");
  for (var c = 0; c < cdiv.length; c++) {
    cdiv[c].style.width = cdiv[c].querySelector(".connector-img").clientWidth + "px";
    var pins = cdiv[c].querySelectorAll(".pin-marker");
    for (var i = 0; i < pins.length; i++) {
      var height = cdiv[c].clientHeight * 0.05;
      pins[i].style.height = height + "px";
      pins[i].style.width = height + "px";
      pins[i].style.marginTop = "-" + (height * 0.5) + "px";
      pins[i].style.marginLeft = "-" + (height * 0.5) + "px";
      pins[i].style.fontSize = (height * 0.5) + "px";
    }
  }
}

window.addEventListener('load', function() {
  for (var c = 0; c < connectorYaml.length; c++) {
    var connector = YAML.parse(connectorYaml[c]);
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
        var pinfo;
        for (var ii = 0; ii < connector.info.pins.length; ii++) {
          if (connector.info.pins[ii].pin == pin.pin) {
            pinfo = connector.info.pins[ii];
            break;
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
        cdiv.appendChild(pdiv);
        addRow(fullTable, connector.pins[i], pdiv);
      }
      adjustMarkers();
    }.bind(null, connector, sdiv, img));
    img.src = connector.info.image.file;
  }
});

window.addEventListener('resize', function() {
  adjustMarkers();
});
