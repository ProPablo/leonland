#include "pch.h"
#include "CppUnitTest.h"
#include "../Application/rendering/Quad.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProject
{
    TEST_CLASS(TestProject)
    {
    public:
        Rect r1{ {0.22,0.34}, {0.5, 0.5} };
        Rect r2{ {0.22,0.34}, {0.5, 0.5} };

        TEST_METHOD(TestMethod1)
        {
            glm::vec2 Pos1 = { 0.22,0.35 };
            Assert::IsTrue(r1.Pos == Pos1);
        }

    };
}
