/* Author(s):
 *   Connor Abbott (connor@abbott.cx)
 *
 * Copyright (c) 2014 Connor Abbott (connor@abbott.cx)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "lower/lower.h"

namespace {

class ir_lower_condition_visitor : public ir_hierarchical_visitor
{
public:
	ir_lower_condition_visitor()
	{
	}
	
	virtual ir_visitor_status visit_enter(ir_discard*);
	virtual ir_visitor_status visit_enter(ir_assignment*);
};
	
}; /* private namespace */


ir_visitor_status ir_lower_condition_visitor::visit_enter(ir_discard* ir)
{
	if (!ir->condition)
		return visit_continue_with_parent;
	
	void* mem_ctx = ralloc_parent(ir);
	
	ir_if* if_stmt = new(mem_ctx) ir_if(ir->condition);
	ir->insert_before(if_stmt);
	if_stmt->then_instructions.push_tail(ir);
	ir->condition = NULL;
	ir->remove();
	
	return visit_continue_with_parent;
}

ir_visitor_status ir_lower_condition_visitor::visit_enter(ir_assignment* ir)
{
	if (!ir->condition)
		return visit_continue_with_parent;
	
	void* mem_ctx = ralloc_parent(ir);
	
	ir_if* if_stmt = new(mem_ctx) ir_if(ir->condition);
	ir->insert_before(if_stmt);
	if_stmt->then_instructions.push_tail(ir);
	ir->condition = NULL;
	ir->remove();
	
	return visit_continue_with_parent;
}

void lima_lower_conditions(exec_list* ir)
{
	ir_lower_condition_visitor v;
	v.run(ir);
}
