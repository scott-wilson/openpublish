# ruff: noqa: D104,F403
from __future__ import annotations

from typing import Dict, List, Union

import openpublish.openpublish
from openpublish.openpublish import *

Value = Union[None, bool, int, float, str, List["Value"], Dict[str, "Value"]]

__doc__ = openpublish.openpublish.__doc__
