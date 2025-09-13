# ruff: noqa: EM101,TRY003

from __future__ import annotations

import pytest

import openpublish

pytestmark = pytest.mark.asyncio


async def test_run_success() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 1)
            self.values.append(("pre_publish", ctx.get("test")))

            return ctx

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 2)
            self.values.append(("publish", ctx.get("test")))

            return ctx

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 3)
            self.values.append(("post_publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()
    result = await openpublish.run(test_publish)

    assert result.get("test") == 1 + 2 + 3
    assert test_publish.values == [
        ("pre_publish", 1),
        ("publish", 3),
        ("post_publish", 6),
    ]


async def test_run_without_pre_post_publish_success() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 2)

            self.values.append(("publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()
    result = await openpublish.run(test_publish)

    assert result.get("test") == 2  # noqa: PLR2004
    assert test_publish.values == [
        ("publish", 2),
    ]


async def test_run_failure_prepublish_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Prepublish failed")

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 2)
            self.values.append(("publish", ctx.get("test")))

            return ctx

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 3)
            self.values.append(("post_publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()

    with pytest.raises(RuntimeError, match="Prepublish failed"):
        await openpublish.run(test_publish)

    assert test_publish.values == []


async def test_run_failure_publish_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 1)
            self.values.append(("pre_publish", ctx.get("test")))

            return ctx

        async def publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Publish failed")

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 3)
            self.values.append(("post_publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()

    with pytest.raises(RuntimeError, match="Publish failed"):
        await openpublish.run(test_publish)

    assert test_publish.values == [("pre_publish", 1)]


async def test_run_failure_postpublish_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 1)
            self.values.append(("pre_publish", ctx.get("test")))

            return ctx

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 2)
            self.values.append(("publish", ctx.get("test")))

            return ctx

        async def post_publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Postpublish failed")

    test_publish = TestPublish()

    with pytest.raises(RuntimeError, match="Postpublish failed"):
        await openpublish.run(test_publish)

    assert test_publish.values == [("pre_publish", 1), ("publish", 3)]


async def test_run_failure_prepublish_rollback_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Prepublish failed")

        async def rollback_pre_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            raise RuntimeError("Prepublish rollback failed")

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 2)
            self.values.append(("publish", ctx.get("test")))

            return ctx

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 3)
            self.values.append(("post_publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()

    with pytest.raises(
        RuntimeError,
        match="Error rolling back: Error while rolling back pre_publish",
    ):
        await openpublish.run(test_publish)

    assert test_publish.values == []


async def test_run_failure_publish_rollback_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 1)
            self.values.append(("pre_publish", ctx.get("test")))

            return ctx

        async def publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Publish failed")

        async def rollback_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            raise RuntimeError("Publish rollback failed")

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 3)
            self.values.append(("post_publish", ctx.get("test")))
            return ctx

    test_publish = TestPublish()

    with pytest.raises(
        RuntimeError,
        match="Error rolling back: Error while rolling back publish",
    ):
        await openpublish.run(test_publish)

    assert test_publish.values == [("pre_publish", 1)]


async def test_run_failure_postpublish_rollback_fail() -> None:
    class TestPublish(openpublish.Publish):
        def __init__(self) -> None:
            self.values = []

        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            ctx.set("test", 1)
            self.values.append(("pre_publish", ctx.get("test")))

            return ctx

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            ctx = context.copy()
            value = ctx.get("test")

            assert isinstance(value, int)

            ctx.set("test", value + 2)
            self.values.append(("publish", ctx.get("test")))

            return ctx

        async def post_publish(
            self,
            context: openpublish.ContextView,  # noqa: ARG002
        ) -> openpublish.Context | openpublish.ContextView:
            raise RuntimeError("Postpublish failed")

        async def rollback_post_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            raise RuntimeError("Postpublish rollback failed")

    test_publish = TestPublish()

    with pytest.raises(
        RuntimeError,
        match="Error rolling back: Error while rolling back post_publish",
    ):
        await openpublish.run(test_publish)

    assert test_publish.values == [("pre_publish", 1), ("publish", 3)]


@pytest.mark.parametrize(
    "raise_prepublish",
    [True, False],
    ids=["prepublish_err", "prepublish_ok"],
)
@pytest.mark.parametrize(
    "raise_publish",
    [True, False],
    ids=["publish_err", "publish_ok"],
)
@pytest.mark.parametrize(
    "raise_postpublish",
    [True, False],
    ids=["postpublish_err", "postpublish_ok"],
)
@pytest.mark.parametrize(
    "raise_prepublish_rollback",
    [True, False],
    ids=["prepublish_rollback_err", "prepublish_rollback_ok"],
)
@pytest.mark.parametrize(
    "raise_publish_rollback",
    [True, False],
    ids=["publish_rollback_err", "publish_rollback_ok"],
)
@pytest.mark.parametrize(
    "raise_postpublish_rollback",
    [True, False],
    ids=["postpublish_rollback_err", "postpublish_rollback_ok"],
)
async def test_run_failure_step_raised_exception(  # noqa: C901, PLR0913
    raise_prepublish: bool,  # noqa: FBT001
    raise_publish: bool,  # noqa: FBT001
    raise_postpublish: bool,  # noqa: FBT001
    raise_prepublish_rollback: bool,  # noqa: FBT001
    raise_publish_rollback: bool,  # noqa: FBT001
    raise_postpublish_rollback: bool,  # noqa: FBT001
) -> None:
    class TestPublish(openpublish.Publish):
        async def pre_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            if raise_prepublish:
                raise RuntimeError("Prepublish failed")

            ctx = context.copy()
            ctx.set("test", 1)

            return ctx

        async def rollback_pre_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            if raise_prepublish_rollback:
                raise RuntimeError("Prepublish rollback failed")

        async def publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            if raise_publish:
                raise RuntimeError("Publish failed")

            ctx = context.copy()
            ctx.set("test", 2)

            return ctx

        async def rollback_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            if raise_publish_rollback:
                raise RuntimeError("Publish rollback failed")

        async def post_publish(
            self,
            context: openpublish.ContextView,
        ) -> openpublish.Context | openpublish.ContextView:
            if raise_postpublish:
                raise RuntimeError("Postpublish failed")

            ctx = context.copy()
            ctx.set("test", 3)

            return ctx

        async def rollback_post_publish(self, context: openpublish.ContextView) -> None:  # noqa: ARG002
            if raise_postpublish_rollback:
                raise RuntimeError("Postpublish rollback failed")

    test_publish = TestPublish()

    if not any(
        [
            raise_prepublish,
            raise_publish,
            raise_postpublish,
            raise_prepublish_rollback,
            raise_publish_rollback,
            raise_postpublish_rollback,
        ],
    ) or not any(
        [
            raise_prepublish,
            raise_publish,
            raise_postpublish,
        ],
    ):
        # Should not raise an exception, since transaction is not rolled back,
        # or rollback is successful
        await openpublish.run(test_publish)
        return

    with pytest.raises(RuntimeError):
        await openpublish.run(test_publish)
