(() => {
  'use strict';

  const rows = Array.from(document.querySelectorAll('.wire-row'));
  const markers = Array.from(document.querySelectorAll('.pin-marker'));
  const pairWires = Array.from(document.querySelectorAll('.pair-wire'));
  const overviewRoutes = Array.from(document.querySelectorAll('.overview-route'));
  const pairs = Array.from(document.querySelectorAll('.connector-pair'));
  const stepFilter = document.getElementById('step-filter');
  const search = document.getElementById('wire-search');
  const zoom = document.getElementById('image-zoom');
  const selection = document.getElementById('selection');
  const count = document.getElementById('visible-count');
  const copyLink = document.getElementById('copy-link');
  const byWire = new Map(rows.map(row => [row.dataset.wire, row]));
  const pairByWire = new Map(pairWires.map(wire => [wire.dataset.wire, wire]));
  let selectedWire = null;

  function address() {
    const url = new URL(window.location.href);
    if (selectedWire) url.searchParams.set('wire', selectedWire);
    else url.searchParams.delete('wire');
    if (stepFilter.value !== 'all') url.searchParams.set('step', stepFilter.value);
    else url.searchParams.delete('step');
    if (search.value.trim()) url.searchParams.set('q', search.value.trim());
    else url.searchParams.delete('q');
    return url;
  }

  function updateAddress() {
    try {
      history.replaceState(null, '', address().toString());
    } catch (_) {
      // Some browsers restrict history changes for local file URLs.
    }
  }

  function showSelection() {
    rows.forEach(row => row.classList.toggle('selected', row.dataset.wire === selectedWire));
    markers.forEach(marker => marker.classList.toggle('selected', marker.dataset.wire === selectedWire));
    pairWires.forEach(wire => wire.classList.toggle('selected', wire.dataset.wire === selectedWire));
    overviewRoutes.forEach(route => route.classList.toggle('selected', route.dataset.wire === selectedWire));
    document.body.classList.toggle('has-selection', Boolean(selectedWire));
    if (!selectedWire) {
      selection.textContent = 'Select a marked pin or a wire row.';
      return;
    }
    const cells = byWire.get(selectedWire).cells;
    selection.textContent = `Step ${cells[0].textContent}: ${cells[2].textContent} ↔ ${cells[3].textContent} · ${cells[4].textContent} · ${cells[1].textContent.trim()}`;
  }

  function filter() {
    const query = search.value.trim().toLocaleLowerCase();
    const visible = new Set();
    rows.forEach(row => {
      const matches = (stepFilter.value === 'all' || row.dataset.step === stepFilter.value)
        && row.dataset.search.includes(query);
      row.hidden = !matches;
      if (matches) visible.add(row.dataset.wire);
    });
    markers.forEach(marker => {
      marker.hidden = marker.dataset.overview !== 'true' && !visible.has(marker.dataset.wire);
    });
    pairWires.forEach(wire => {
      wire.hidden = !visible.has(wire.dataset.wire);
    });
    pairs.forEach(pair => {
      pair.hidden = !Array.from(pair.querySelectorAll('.pair-wire')).some(wire => !wire.hidden);
    });
    count.textContent = `${visible.size} of ${rows.length} wires shown`;
    if (selectedWire && !visible.has(selectedWire)) selectedWire = null;
    showSelection();
    updateAddress();
  }

  function select(wire) {
    if (!byWire.has(wire)) return;
    if (byWire.get(wire).hidden) {
      stepFilter.value = 'all';
      search.value = '';
      filter();
    }
    selectedWire = wire;
    showSelection();
    updateAddress();
  }

  function hover(wire, on) {
    rows.forEach(row => {
      if (row.dataset.wire === wire) row.classList.toggle('hovered', on);
    });
    markers.forEach(marker => {
      if (marker.dataset.wire === wire) marker.classList.toggle('hovered', on);
    });
    pairWires.forEach(pairWire => {
      if (pairWire.dataset.wire === wire) pairWire.classList.toggle('hovered', on);
    });
    overviewRoutes.forEach(route => {
      if (route.dataset.wire === wire) route.classList.toggle('hovered', on);
    });
  }

  [...rows, ...markers, ...pairWires, ...overviewRoutes].forEach(item => {
    item.addEventListener('click', () => {
      select(item.dataset.wire);
      if (item.tagName === 'TR') pairByWire.get(item.dataset.wire).scrollIntoView({ block: 'center' });
    });
    item.addEventListener('mouseenter', () => hover(item.dataset.wire, true));
    item.addEventListener('mouseleave', () => hover(item.dataset.wire, false));
    if (item.tagName === 'TR') {
      item.addEventListener('keydown', event => {
        if (event.key === 'Enter' || event.key === ' ') {
          event.preventDefault();
          select(item.dataset.wire);
          pairByWire.get(item.dataset.wire).scrollIntoView({ block: 'center' });
        }
      });
    }
  });

  stepFilter.addEventListener('change', filter);
  search.addEventListener('input', filter);
  zoom.addEventListener('input', () => {
    document.documentElement.style.setProperty('--zoom', `${zoom.value}%`);
  });
  copyLink.addEventListener('click', async () => {
    const link = address().toString();
    try {
      await navigator.clipboard.writeText(link);
      copyLink.textContent = 'Link copied';
      window.setTimeout(() => { copyLink.textContent = 'Copy selection link'; }, 1800);
    } catch (_) {
      window.prompt('Copy this link:', link);
    }
  });

  const params = new URLSearchParams(window.location.search);
  const requestedStep = params.get('step');
  if ([...stepFilter.options].some(option => option.value === requestedStep)) stepFilter.value = requestedStep;
  search.value = params.get('q') || '';
  filter();
  const requestedWire = params.get('wire');
  if (requestedWire && byWire.has(requestedWire)) {
    select(requestedWire);
    pairByWire.get(requestedWire).scrollIntoView({ block: 'center' });
  }
})();
