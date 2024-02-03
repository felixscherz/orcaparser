import logging
from dataclasses import dataclass
from typing import Any

logger = logging.getLogger()


@dataclass
class Element:
    weight: float
    real: float
    imag: float
    root: int
    spin: float
    m_s: int


@dataclass
class State:
    n: int
    elements: list[Element]



{
    "n": 0,
    "elements": [{"E": 0.99999, "Real": -0.52498, "Imag": 0.85111, "Root": 0, "Spin": 0, "Ms": 0}],
}

def element_from_dict(element: dict[str, Any]) -> Element:
    return Element(weight=element["E"], real=element["Real"], imag=element["Imag"], root=element["Root"], spin=element["Spin"], m_s=element["Ms"])

def state_from_dict(state: dict[str, Any]) -> State:
    elements = [element_from_dict(d) for d in state["elements"]]
    return State(n=state["n"], elements=elements)


def test_import_from_parser():
    from orcaparser.parser import parse_states

    result = parse_states("ch2o_soc_tddft.out")

    logger.warning(result)


def test_import_and_convert():
    from orcaparser.parser import parse_states

    result = parse_states("ch2o_soc_tddft.out")
    states = [state_from_dict(d) for d in result]

    for state in states:
        assert isinstance(state, State)
        print(state)

    logger.warning(result)
