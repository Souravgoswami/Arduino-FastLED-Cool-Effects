
def gradient(*arg_colours,
	exclude_spaces: true,
	bg: false,
	bold: false,
	italic: false,
	underline: false,
	blink: false,
	strikethrough: false,
	double_underline: false,
	overline: false
	)

	temp = ''
	flatten_colours = arg_colours.flatten

	all_rgbs = arg_colours

	# r, g, b => starting r, g, b
	# r2, g2, b2 => stopping r, g, b
	r, g, b = *all_rgbs[0]
	r2, g2, b2 = *all_rgbs[1]
	rotate = all_rgbs.length > 2

	_r, _g, _b = r, g, b

	len = 30
	n_variable = len
	# colour operator, colour value
	#
	# r_op, g_op, b_op are also flags to determine
	# if the r, g, and b values respectively should change or not
	# For example, if the given blues are equal, the b_op is nil
	# So it won't change the colour in the ouput
	r_op = r_val  = nil
	g_op = g_val = nil
	b_op = b_val = nil

	if r2 > r
		r_op, r_val = :+, r2.-(r).fdiv(n_variable)
	elsif r2 < r
		r_op, r_val = :-, r.-(r2).fdiv(n_variable)
	end

	if g2 > g
		g_op, g_val = :+, g2.-(g).fdiv(n_variable)
	elsif g2 < g
		g_op, g_val = :-, g.-(g2).fdiv(n_variable)
	end

	if b2 > b
		b_op, b_val = :+, b2.-(b).fdiv(n_variable)
	elsif b2 < b
		b_op, b_val = :-, b.-(b2).fdiv(n_variable)
	end

	# To avoid the value getting adding | subtracted from the initial character
	_r = _r.send(r_op, r_val * -1) if r_op
	_g = _g.send(g_op, g_val * -1) if g_op
	_b = _b.send(b_op, b_val * -1) if b_op

	i = -1
	while (i += 1) < len
		_r = _r.send(r_op, r_val) if r_op
		_g = _g.send(g_op, g_val) if g_op
		_b = _b.send(b_op, b_val) if b_op

		r_to_i = _r.to_i
		g_to_i = _g.to_i
		b_to_i = _b.to_i

		clamped_r = r_to_i < 0 ? 0 : r_to_i > 255 ? 255 : r_to_i
		clamped_g = g_to_i < 0 ? 0 : g_to_i > 255 ? 255 : g_to_i
		clamped_b = b_to_i < 0 ? 0 : b_to_i > 255 ? 255 : b_to_i

		temp << "0x#{"%02x" % clamped_r}#{"%02x" % clamped_g}#{"%02x" % clamped_b}, "
	end
	temp
end



##
# = multi_gradient(*n_arg_colours, bg: false, exclude_spaces: true, bold: false, blink: false)    # => string or nil
#
# Accepts n number of colours. Example:
#    'Hello world this is multi_gradient()'.multi_gradient('3eb', '55f', 'f55', 'fa0')
#
# In this example, multi_gradient() paints the string with 4 colours in one line.
#
# It Splits up a string with the Calls String#gradient() with the given number of colours
# So each call to multi_gradient() involves many calls to String#gradient().
# Hence it's slower than String#gradient()
def multi_gradient(*colours)
	len = colours.length
	raise ArgumentError, "Minimum two colours are required, given #{len}" if len < 2

	div = len - 1
	div_1 = div
	ret = ''

	len, c = len.fdiv(div).round, colours.dup
	counter, i, j = -1, -1, 0
	ch = ''

	while (i += 1) < 255 do
		counter += 1

#		p [counter, len, j, div_1]

		if counter == len && j < div_1
			counter, j = 0, j + 1

			p [c[0], c[1]]
			ret << gradient([c[0], c[1]])

			c.rotate!
			ch.clear
		end
	end

	ret
end

puts gradient([0, 255, 255], [0, 0, 255]) +
	gradient([0, 0, 255], [255, 0, 80]) +
	gradient([255, 0, 80], [255, 0, 255]) +
	gradient([255, 0, 255], [0, 255, 0]) +
	gradient([0, 255, 0], [0, 255, 255])
