#include "pch.h"
#include "CppUnitTest.h"
#include "../Application/rendering/Quad.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace TestProject
{
    TEST_CLASS(RectTests)
    {
    public:
        Rect r1{ {0.22,0.34}, {0.5, 0.5} };
        Rect r2{ {0.22,0.34}, {0.5, 0.5} };

        TEST_METHOD(RectTestPosEquality)
        {
            glm::vec2 Pos1 = { 0.22,0.34 };
            Assert::IsTrue(r1.Pos == Pos1);
        }

        TEST_METHOD(TestMethod2)
        {
            glm::vec2 Pos1 = { 0.22,0.35 };
            Assert::IsTrue(r1.Pos == Pos1);
        }
    };
}
