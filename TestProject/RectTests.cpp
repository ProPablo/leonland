#include "pch.h"
#include "CppUnitTest.h"
#include "../Application/rendering/Quad.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace TestProject
{
    TEST_CLASS(RectTests)
    {
    public:
        glm::vec2 pos1 = { 0.230000004,0.219999999 };
        Rect r1{ {0.22,0.34}, {0.5, 0.5} };
        Rect r2{ {0.22,0.34}, {0.5, 0.5} };

        TEST_METHOD(RectTestPosEquality)
        {
            glm::vec2 Pos1 = { 0.22,0.34 };
            Assert::IsTrue(r1.Pos == Pos1);
        }

        TEST_METHOD(PointContained)
        {
            Assert::IsTrue(r1.Contains(pos1));
        }


        TEST_METHOD(RectContained)
        {
            Assert::IsTrue(r1.Contains(r2));
        }

        TEST_METHOD(RectOverlap)
        {
            Assert::IsTrue(r1.Overlaps(r2));
        }
    };
}
