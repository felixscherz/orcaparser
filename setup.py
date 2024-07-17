from setuptools import Extension
from setuptools import setup

ext_modules = [
    Extension(
        "orcaparser.parser",
        ["src/orcaparser/parser/orcaparser.c", "src/orcaparser/parser/main.c"],
        include_dirs=["./src/orcaparser/parser"],
    )
]


def main():
    setup(
        name="orcaparser",
        version="0.1.0",
        packages=["src/orcaparser"],
        ext_modules=ext_modules,
    )


if __name__ == "__main__":
    main()
