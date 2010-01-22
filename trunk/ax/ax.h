/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Axonlib.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * \brief test template class
 */

/**
 * \brief test template class
 *
 * use this as a starting point when creating new classes
 *
 */

#ifndef ax_included
#define ax_included
//----------------------------------------------------------------------

class ax// : public ax
{
  private:

    float priv_var; ///< description..

  protected:

    /** longer sentence here blablabladsg afgb sdfg
        longer sentence here blablabladsg afgb sdfg
        longer sentence here blablabladsg afgb sdfg */

    float prot_var; ///< protvar

  public:

    /**
     * constructor
     */

    ax() {}

    /**
     * destructor
     */

    virtual ~ax() {}

    /**
     * test function
     * blablabla...
     * ...
     *
     * @param[in] arg1 int
     */

    virtual void func1(int arg1) {}

    /**
     * test function 2
     * @param[in] arg1 int
     * @param[out] result int
     */

    virtual int func2(int arg1) { return 0; }

};

//----------------------------------------------------------------------
#endif
