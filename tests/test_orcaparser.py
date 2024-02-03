import logging
logger = logging.getLogger()


def test_import_from_parser():
    from orcaparser.parser2 import parse_states
    result = parse_states("ch2o_soc_tddft.out")
    logger.warning(result)



