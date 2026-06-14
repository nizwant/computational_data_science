import setuptools

setuptools.setup(
    name="PeerChat",
    packages=setuptools.find_packages(),
    ext_modules=[
        setuptools.Extension(
            "PeerChat.peer_chat",
            sources=[
                "src/py_peer_chat.c",
                "src/client_lib.c",
                "src/c_common.c",
            ],
            include_dirs=["src"],
        )
    ],
)