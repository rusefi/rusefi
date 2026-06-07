"""
.ini file resolution helpers — mirrors the Java SignatureHelper logic in Python.

Lets scripts check .ini availability *before* launching a Java MCP server,
avoiding opaque IllegalStateException errors.

Usage::

    from java_console.mcp_python import ensure_ini, find_ini, ini_url, parse_signature

    sig = "rusEFI master.2026.06.07.purple-gateway.753206531"
    path = ensure_ini(sig)          # find or download; raises if unavailable
    cached = find_ini(sig)          # None if not in local cache
    url = ini_url(sig)              # HTTP download URL
    parts = parse_signature(sig)    # dict with branch/year/month/day/target/hash
"""

from pathlib import Path
import urllib.request

RUSEFI_SETTINGS = Path.home() / ".rusEFI"
INI_CACHE = RUSEFI_SETTINGS / "ini_database"
INI_URL_TEMPLATE = "https://rusefi.com/online/ini/rusefi/{branch}/{year}/{month}/{day}/{target}/{hash}.ini"


def parse_signature(sig: str) -> dict | None:
    """Parse ``'rusEFI branch.year.month.day.target.hash'`` into components.

    Returns a dict with keys ``branch``, ``year``, ``month``, ``day``,
    ``target``, ``hash``, or ``None`` if the signature is malformed.
    """
    if not sig or not sig.startswith("rusEFI "):
        return None
    parts = sig[7:].strip().split(".")
    if len(parts) != 6:
        return None
    return dict(zip(["branch", "year", "month", "day", "target", "hash"], parts))


def find_ini(signature: str) -> Path | None:
    """Check the local console cache for the ``.ini`` matching *signature*.

    Returns the ``Path`` if found (and > 10 KB), otherwise ``None``.
    """
    s = parse_signature(signature)
    if not s:
        return None
    cached = INI_CACHE / f"{s['hash']}.ini"
    if cached.exists() and cached.stat().st_size > 10000:
        return cached
    return None


def ini_url(signature: str) -> str | None:
    """Build the HTTP download URL for this signature's ``.ini``."""
    s = parse_signature(signature)
    if not s:
        return None
    return INI_URL_TEMPLATE.format(**s)


def ensure_ini(signature: str) -> Path:
    """Find or download the ``.ini``; raise if unavailable.

    Resolution order:

    1. Local console cache (``~/.rusEFI/ini_database/{hash}.ini``)
    2. HTTP download from ``rusefi.com``

    Raises ``ValueError`` for unparseable signatures and ``FileNotFoundError``
    when the ``.ini`` cannot be obtained.
    """
    path = find_ini(signature)
    if path:
        return path
    s = parse_signature(signature)
    if not s:
        raise ValueError(f"Cannot parse signature: {signature}")
    url = INI_URL_TEMPLATE.format(**s)
    dest = INI_CACHE / f"{s['hash']}.ini"
    dest.parent.mkdir(parents=True, exist_ok=True)
    try:
        urllib.request.urlretrieve(url, dest)
        return dest
    except Exception as e:
        raise FileNotFoundError(
            f".ini not in cache ({dest}) and download failed ({url}): {e}\n"
            f"Place the .ini manually or check the URL."
        )
