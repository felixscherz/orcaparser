from setuptools import Extension
from setuptools import setup

# ext_modules = [
#     Extension(
#         "orcaparser",
#         ["orcaparser/orcaparser.c", "orcaparser/main.c"],
#         include_dirs=["./orcaparser"],
#     )
# ]
ext_modules = [Extension("orcaparser.parser", ["orcaparser.c", "main.c"], include_dirs=["."])]


def main():
    setup(
        name="orcaparser",
        version="0.1.0",
        ext_modules=ext_modules,
    )


if __name__ == "__main__":
    main()
