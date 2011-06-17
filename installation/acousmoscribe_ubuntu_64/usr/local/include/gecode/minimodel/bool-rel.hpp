/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2005
 *
 *  Last modified:
 *     $Date: 2009-01-20 23:44:27 +0100 (Tue, 20 Jan 2009) $ by $Author: schulte $
 *     $Revision: 8082 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

namespace Gecode {

  /*
   * Operations for Boolean relations
   *
   */
  forceinline
  BoolRel::BoolRel(const BoolExpr& e0, bool t0)
    : e(e0), t(t0) {}

  forceinline void
  BoolRel::post(Space& home, IntConLevel icl) const {
    e.post(home,t,icl);
  }

  /*
   * Construction of linear relations
   *
   */
  inline BoolRel
  tt(const BoolExpr& e) {
    return BoolRel(e,true);
  }
  inline BoolRel
  ff(const BoolExpr& e) {
    return BoolRel(e,false);
  }

  /*
   * Posting
   *
   */
  inline void
  post(Space& home, const BoolRel& r, IntConLevel icl) {
    if (home.failed()) return;
    r.post(home,icl);
  }

}

// STATISTICS: minimodel-any